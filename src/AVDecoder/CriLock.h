#ifndef CriLock_h__
#define CriLock_h__

class CCriLock
{
public:
	CCriLock(CRITICAL_SECTION &rcri);
	~CCriLock(void);

	CRITICAL_SECTION &m_rcri;
};

#endif // CriLock_h__
