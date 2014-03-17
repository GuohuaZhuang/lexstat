
#ifndef _LIBXML2_CURL_H_
#define _LIBXML2_CURL_H_

namespace lexstat {
class SingletonLibxml2 {
public:
	static SingletonLibxml2* GetInstance(void) {
		if (m_pInstance == 0) { m_pInstance = new SingletonLibxml2(); }
		return m_pInstance;
	}
	
private:
	static SingletonLibxml2* m_pInstance;
	class MemRelease {
	public:
		MemRelease() { SingletonLibxml2::GetInstance(); }
		~MemRelease() {
			if (SingletonLibxml2::m_pInstance) {
				delete SingletonLibxml2::m_pInstance;
			}
		}
	};
	static MemRelease m_MemRelease;
	SingletonLibxml2();
	
public:
	~SingletonLibxml2();
    static int GetExtract(const char* html, const int htmlsiz, 
        char** pext, int* pextsiz);
	
private:
};

} // namespace

#endif // #define _LIBXML2_CURL_H_

