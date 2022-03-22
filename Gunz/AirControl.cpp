/*
	Addon Name: AirControl
	Revision: r3
	Author: Franc[e]sco
*/
 
#include "stdafx.h"
#include "AirControl.h"
 
namespace ZAddons 
{
	namespace Francesco
	{
		std::auto_ptr<AirControl> AirControl::instance; // singleton
	 
		// vertical / horizontal amount of air control
		const float AirControl::vamount = 25.f;
		const float AirControl::hamount = 25.f;
	 
		AirControl *AirControl::Get()
		{
			if (!instance.get())
				instance = std::auto_ptr<AirControl>(new AirControl);
	 
			return instance.get();
		}
	 
		AirControl::AirControl()
			: bEnabled(false)
		{
			// empty
		}
	 
		AirControl::~AirControl()
		{
			// empty
		}
	 
		void AirControl::Enable(bool bEnabled)
		{
			this->bEnabled = bEnabled;
		}
	 
		bool AirControl::Enabled()
		{
			return bEnabled;
		}
	 
		bool AirControl::CanReset(ZCharaterStatusBitPacking *status)
		{
			// reset velocity before handling player input only if the player is on the ground
			return !bEnabled || status->m_bLand;
		}
	 
		void AirControl::Denormalize(ZCharaterStatusBitPacking *status, rvector *forward, rvector *right)
		{
			// basically, when jumping m_Accel is not normalized and the movement vectors are de-normalized by 
			// multiplying them by a multiplier so that the player can gradually change direction
			if (bEnabled && !status->m_bLand)
			{
				*forward *= vamount;
				*right *= hamount;
			}
		}
	}
}