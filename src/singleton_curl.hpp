
#ifndef _SINGLETON_CURL_H_
#define _SINGLETON_CURL_H_

#include "Common.h"
#include "ext_string.h"

using namespace std;

namespace lexstat {
class SingletonCurl {
public:
	static SingletonCurl* GetInstance(void) {
		if (m_pInstance == 0) { m_pInstance = new Instance(); }
		return m_pInstance;
	}
	
private:
	static SingletonCurl* m_pInstance;
	class MemRelease {
	public:
		MemRelease() { SingletonCurl::GetInstance(); }
		~MemRelease() {
			if (SingletonCurl::m_pInstance) {
				delete SingletonCurl::m_pInstance;
			}
		}
	};
	static MemRelease m_MemRelease;
	SingletonCurl();
	
public:
	~SingletonCurl();
	static int GetResult(const std::ext_string& str);
	
private:
	static map<ext_string, int> m_map_trim;
};

} // namespace

#endif // #define _SINGLETON_CURL_H_

