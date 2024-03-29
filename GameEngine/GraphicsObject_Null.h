#ifndef GRAPHICS_OBJECT_NULL_H
#define GRAPHICS_OBJECT_NULL_H

#include "GraphicsObject.h"


class GraphicsObject_Null :public GraphicsObject
{
public:
	GraphicsObject_Null(Model *model, ShaderObject *shaderObject);

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here



};
#endif