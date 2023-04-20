#include "NavigationManager.h"
#include "../Thread/NavigationThread.h"

CNavigationManager::CNavigationManager()
    : m_Owner(nullptr)
{
}

CNavigationManager::~CNavigationManager()
{
}

void CNavigationManager::Start()
{
}

bool CNavigationManager::Init()
{
    return true;
}

void CNavigationManager::Move(CNavigationAgent* agent, const Vector2& end)
{
    if (m_vecNavThread.empty())
    {
        return;
    }
    // ������ �ִ� �����忡�� ���� ���� ���� ����ִ� �����带 �Ǵ��Ѵ�.
    int selectThread = 0;
    int workCount = m_vecNavThread[0]->GetWorkCount();
    if (workCount > 0)
    {
        size_t  size = m_vecNavThread.size();
        for (size_t i = 1; i < size; ++i)
        {
            if (workCount > m_vecNavThread[i]->GetWorkCount())
            {
                workCount = m_vecNavThread[i]->GetWorkCount();
                selectThread = (int)i;
                if (workCount == 0)
                {
                    break;
                }
            }
        }
    }
    m_vecNavThread[selectThread]->AddInputData(agent, end);
}
