#pragma once
 
/*
	Air Control
	======================================================
	A modification to the game's physics so that players 
	can gradually change direction mid-air
	======================================================
	Addon Name: AirControl
	Author: Franc[e]sco
*/
 
#include "ZCharacter.h"
#include <memory>
 
namespace ZAddons 
{
	namespace Francesco
	{
		class AirControl
		{
		public:
			virtual ~AirControl();
			static AirControl *Get();
			void Enable(bool bEnabled);
			bool Enabled();
			bool CanReset(ZCharaterStatusBitPacking *status); // lol they typo'd character
			void Denormalize(ZCharaterStatusBitPacking *status, rvector *forward, rvector *right);
	 
		protected:
			static std::auto_ptr<AirControl> instance;
			static const float vamount; // vertical amount of air control
			static const float hamount; // horizontal amount
	 
			bool bEnabled;
	 
			AirControl();
		};
	}
}