#include "Subject.h"
#include "Observer.h"

using dae::Observer, dae::Subject;

void Subject::Notify(const void* pEntity, const std::string& event) const
{
    for (Observer* pObserver: m_pObservers)
    {
        pObserver->OnNotify(pEntity, event);
    }
}

void Subject::AddObserver(Observer* pObserver)
{
    m_pObservers.push_back(pObserver);
}

void Subject::RemoveObserver(Observer* pObserver)
{
    m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserver), m_pObservers.end());
}
