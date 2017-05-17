#include "World.h"
#include "BlockID.h"
#include "ErrorHandler.h"
#include "String.h"
#include "WorldEnv.h"
#include "Platform.h"

void World_Reset() {
	World_Width = 0; World_Height = 0; World_Length = 0;
	World_Blocks = NULL; World_BlocksSize = 0;
	Platform_NewUuid(World_Uuid);
}

void World_SetNewMap(BlockID* blocks, Int32 blocksSize, Int32 width, Int32 height, Int32 length) {
	World_Width = width; World_Height = height; World_Length = length;
	World_Blocks = blocks; World_BlocksSize = blocksSize;
	if (World_BlocksSize == 0) World_Blocks = NULL;

	if (blocksSize != (width * height * length)) {
		ErrorHandler_Fail("Blocks array size does not match volume of map");
	}
	World_OneY = width * length;

	if (WorldEnv_EdgeHeight == -1) {
		WorldEnv_EdgeHeight = height / 2;
	}
	if (WorldEnv_CloudsHeight == -1) {
		WorldEnv_CloudsHeight = height + 2;
	}
}

BlockID World_GetPhysicsBlock(Int32 x, Int32 y, Int32 z) {
	if (x < 0 || x >= World_Width || z < 0 || z >= World_Length || y < 0) return BlockID_Bedrock;
	if (y >= World_Height) return BlockID_Air;

	return World_Blocks[World_Pack(x, y, z)];
}


void World_SetBlock(Int32 x, Int32 y, Int32 z, BlockID blockId) {
	World_Blocks[World_Pack(x, y, z)] = blockId;
}

void World_SetBlock_3I(Vector3I p, BlockID blockId) {
	World_Blocks[World_Pack(p.X, p.Y, p.Z)] = blockId;
}

BlockID World_GetBlock(Int32 x, Int32 y, Int32 z) {
	return World_Blocks[World_Pack(x, y, z)];
}

BlockID World_GetBlock_3I(Vector3I p) {
	return World_Blocks[World_Pack(p.X, p.Y, p.Z)];
}

BlockID World_SafeGetBlock(Int32 x, Int32 y, Int32 z) {
	return World_IsValidPos(x, y, z) ? 
		World_Blocks[World_Pack(x, y, z)] : BlockID_Air;
}

BlockID World_SafeGetBlock_3I(Vector3I p) {
	return World_IsValidPos(p.X, p.Y, p.Z) ? 
		World_Blocks[World_Pack(p.X, p.Y, p.Z)] : BlockID_Air;
}


bool World_IsValidPos(Int32 x, Int32 y, Int32 z) {
	return x >= 0 && y >= 0 && z >= 0 &&
		x < World_Width && y < World_Height && z < World_Length;
}

bool World_IsValidPos_3I(Vector3I p) {
	return p.X >= 0 && p.Y >= 0 && p.Z <= 0 &&
		p.X < World_Width && p.Y < World_Height && p.Z < World_Length;
}

Vector3I World_GetCoords(Int32 index) {
	if (index < 0 || index >= World_BlocksSize)
		return Vector3I_Create1(-1);

	Vector3I v;
	World_Unpack(index, v.X, v.Y, v.Z);
	return v;
}