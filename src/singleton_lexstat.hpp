
#ifndef _SINGLETON_LEXSTAT_H_
#define _SINGLETON_LEXSTAT_H_

namespace lexstat {
class SingletonLexstat {
public:
	static SingletonLexstat* GetInstance(void) {
		if (m_pInstance == 0) { m_pInstance = new SingletonLexstat(); }
		return m_pInstance;
	}
	
private:
	static SingletonLexstat* m_pInstance;
	class MemRelease {
	public:
		MemRelease() { SingletonLexstat::GetInstance(); }
		~MemRelease() {
			if (SingletonLexstat::m_pInstance) {
				delete SingletonLexstat::m_pInstance;
			}
		}
	};
	static MemRelease m_MemRelease;
	SingletonLexstat();
	
public:
	~SingletonLexstat();
    static int GetResult(const char* ext, const int extsiz, 
        void** plexres, int* plexressiz);
	
private:
    static void* m_lexstat;
};

} // namespace

#endif // #define _SINGLETON_LEXSTAT_H_

