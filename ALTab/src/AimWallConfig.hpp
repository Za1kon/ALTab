// File: AimWallConfig.hpp
#pragma once

// Estructura que contiene la configuración del AimWall
struct AimWallConfig {
	bool sizeShrink = false;
	bool microshot = false;
	bool locked = false;

	enum Mode {
		Multishot,
		Classic3x3,
		HorizontalRow,
		FlickPairs,
		LongTravel
	} mode = Multishot;

	int count = 5; // Solo aplica en Multishot
};