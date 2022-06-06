#include "pch.h"
#include "E_Tato.h"

E_Tato::E_Tato() :
	Enemy("E_Tato.png", 10, 90, 1.f)
{
	m_IdleDist = 50;
	m_RunningDist = 5;
}