//
// Created by henry on 10/8/23.
//

#ifndef HAGAME2_BEHAVIORTREE_H
#define HAGAME2_BEHAVIORTREE_H

#include <memory>
#include "../math/aliases.h"
#include "../core/object.h"
#include "../core/entity.h"
#include "../utils/steadyRate.h"
#include "../utils/variant.h"

namespace hg::utils {

    template <typename GameState>
    class BehaviorTree;

    namespace bt {

        using data_context_t = std::unordered_map<uuid_t, variant>;

        inline bool HasData(data_context_t* ctx, uuid_t member) {
            return ctx->find(member) != ctx->end();
        }

        template <typename T>
        T GetData(data_context_t* ctx, uuid_t member) {
            if (!HasData(ctx, member)) {
                throw std::runtime_error("Data member does not exist: " + std::to_string(member));
            }
            return std::get<T>(ctx->at(member));
        }

        template <typename T>
        void SetData(data_context_t* ctx, const uuid_t& member, T value) {
            if (ctx->find(member) == ctx->end()) {
                ctx->insert(std::make_pair(member, value));
            } else {
                ctx->at(member).emplace<T>(value);
            }
        }

        enum class Status {
            Success,
            Failure,
            Running,
        };

        template <typename GameState>
        class Node;

        template <typename T, typename GameState>
        concept IsNode = std::is_base_of<Node<GameState>, T>::value;

        template <typename GameState = void>
        class Node : public Object {
            public:

                friend class BehaviorTree<GameState>;

                void clear() {
                    m_children.clear();
                }

                Status tick(double dt, GameState* state, data_context_t* ctx, bool firstTick) {
                    if (firstTick) {
                        init(state, ctx);
                    }

                    return process(dt, state, ctx);
                }

            protected:

                std::string toString() const {
                    return "Node<" + std::to_string(id()) + ">";
                }

                virtual void init(GameState* state, data_context_t* ctx) {};
                virtual Status process(double dt, GameState* state, data_context_t* ctx) = 0;

                BehaviorTree<GameState>* m_tree;
                Node* m_parent;
                std::vector<std::shared_ptr<Node>> m_children;
        };

        template <typename GameState>
        class Service : public SteadyRate, public Node<GameState> {
        public:

            friend class BehaviorTree<GameState>;

        protected:

            void onTick(double dt) override {
                this->process(dt, m_state, m_ctx);
            }

        private:

            GameState* m_state;
            hg::utils::bt::data_context_t * m_ctx;

        };

        template <typename T, typename GameState>
        concept IsService = std::is_base_of<Service<GameState>, T>::value;
    }

    template <typename GameState>
    class BehaviorTree {
    public:

        template <bt::IsNode<GameState> NodeType, class... Args>
        bt::Node<GameState> * setRoot(Args... args) {
            m_root = std::make_unique<NodeType>(std::forward<Args>(args)...);
            m_root->m_tree = this;
            return m_root.get();
        }

        template <bt::IsService<GameState> NodeType, class... Args>
        bt::Service<GameState>* addService(Args... args) {
            auto service = std::make_shared<NodeType>(std::forward<Args>(args)...);
            service->m_tree = this;
            m_services.push_back(service);
            return service.get();
        }

        template <bt::IsNode<GameState> ChildType, bt::IsNode<GameState> ParentType, class... Args>
        ChildType* addChild(ParentType* parent, Args &&... args) {
            auto node = std::make_shared<ChildType>(std::forward<Args>(args)...);
            parent->m_children.push_back(node);
            node->m_parent = parent;
            node->m_tree = this;
            return node.get();
        }

        template <bt::IsNode<GameState> ParentType, bt::IsNode<GameState> ChildType>
        void removeChild(ParentType* parent, ChildType* child) {
            parent->m_children.erase(
                std::remove_if(parent->m_children.begin(), parent->m_children.end(), [child](const auto& other) {
                    return child->id() == other->id();
                }), parent->m_children.end());
        }

        bt::Status tick(double dt, GameState* state) {
            if (!m_root) {
                return bt::Status::Failure;
            }

            for (const auto& service : m_services) {
                service->m_state = state;
                service->m_ctx = &m_context;
                service->update(dt);
            }
            if (m_current) {
                bt::Status status = m_current->tick(dt, state, &m_context, false);

                if (status == bt::Status::Running) {
                    return bt::Status::Running;
                }

                if (m_current->m_parent) {
                    m_current = m_current->m_parent;
                } else {
                    m_current = nullptr;
                }

                return status;
            }

            return m_root->tick(dt, state, &m_context, true);
        }

        bt::Node<GameState>* root() {
            return m_root.get();
        }

        bt::data_context_t* context() {
            return &m_context;
        }

        void setCurrent(bt::Node<GameState>* node) {
            m_current = node;
        }

        bt::Node<GameState>* getCurrent() const {
            return m_current;
        }

    private:

        bt::Node<GameState>* m_current = nullptr;
        std::unique_ptr<bt::Node<GameState>> m_root;
        std::vector<std::shared_ptr<bt::Service<GameState>>> m_services;
        bt::data_context_t m_context;
    };

    namespace bt {
        // Composite Nodes

        template <typename GameState>
        class Selector  : public Node<GameState> {
        protected:

            void init(GameState* state, data_context_t* ctx) override {
                m_index = 0;
            }

            Status process(double dt, GameState* state, data_context_t* ctx) override {

                if (m_index >= this->m_children.size()) {
                    this->m_tree->setCurrent(this);
                    return Status::Failure;
                }

                auto child = this->m_children[m_index++];

                Status status = child->tick(dt, state, ctx, true);

                if (status == Status::Running) {
                    this->m_tree->setCurrent(child.get());
                    return status;
                }

                this->m_tree->setCurrent(this);

                if (status == Status::Success) {
                    return status;
                } else {
                    return Status::Running;
                }
            }

            std::string toString() const override {
                return "Selector<" + std::to_string(this->id()) + ">";
            }
        private:

            int m_index = 0;

        };

        template <typename GameState>
        class Sequence : public Node<GameState> {
        protected:

            void init(GameState* state, data_context_t* ctx) override {
                m_index = 0;
            }

            Status process(double dt, GameState* state, data_context_t* ctx) override {

                if (m_index >= this->m_children.size()) {
                    this->m_tree->setCurrent(this);
                    return Status::Success;
                }

                auto child = this->m_children[m_index++];

                Status status = child->tick(dt, state, ctx, true);

                if (status == Status::Running) {
                    this->m_tree->setCurrent(child.get());
                    return status;
                }

                this->m_tree->setCurrent(this);

                if (status == Status::Failure) {
                    //this->m_tree->setCurrent(this->m_parent);
                    return status;
                } else {
                    //this->m_tree->setCurrent(this->m_parent);
                    return Status::Running;
                }
            }

            std::string toString() const override {
                return "Sequence<" + std::to_string(this->id()) + ">";
            }
        private:

            int m_index = 0;

        };

        // Decorators
        template <typename GameState>
        class Inverter : public Node<GameState> {
        protected:
            Status process(double dt, GameState* state, data_context_t* ctx) override {
                for (const auto& child : this->m_children) {
                    Status status = child->tick(state, ctx);
                    if (status == Status::Running || status == Status::Failure) {
                        return status;
                    }
                }
                return Status::Success;
            }

            std::string toString() const {
                return "Sequence<" + std::to_string(this->id()) + ">";
            }
        };

    }

}

#endif //HAGAME2_BEHAVIORTREE_H
