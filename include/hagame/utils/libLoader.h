
#include <string>

#ifdef __APPLE__

// UH OHHHH
// TODO: Someone build me!

error;

#endif

#ifdef __UNIX__
#include <dlfcn.h>
#endif

#ifdef _WIN32
#include "windows.h"
#endif

namespace hg::utils {
	class LibLoader {
	public:

		LibLoader(std::string path);
		~LibLoader();

		void reload();

		template <typename T>
		T loadFunction(std::string name) {
			if (m_library == nullptr) {
				throw std::runtime_error("Library is not open!");
			}
			#ifdef __UNIX__
				return reinterpret_cast<T>(dlsym(m_library, name.c_str()));
			#endif

			#ifdef _WIN32
				return reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(m_library), name.c_str()));
			#endif
		}

	private:

		std::string m_path;
		void* m_library;

	};
}