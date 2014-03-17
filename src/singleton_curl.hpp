
#ifndef _SINGLETON_CURL_H_
#define _SINGLETON_CURL_H_

namespace lexstat {
class SingletonCurl {
public:
	static SingletonCurl* GetInstance(void) {
		if (m_pInstance == 0) { m_pInstance = new SingletonCurl(); }
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
    static int GetPage(const char* url, const int urlsiz, 
        char** phtml, int* phtmlsiz);
	
private:
    static void* m_curl;
};

} // namespace

#endif // #define _SINGLETON_CURL_H_

