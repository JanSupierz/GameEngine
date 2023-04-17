#pragma once
#include "Subject.h"

namespace dae
{
    class BombComponent;

    class BombExplodedEvent final:
        public Subject<BombExplodedEvent>
    {
    public:
        void operator()(BombComponent* pExploded);
        BombComponent* GetExpoloded() const;

    private:
        BombComponent* m_pExploded{};
    };
}

