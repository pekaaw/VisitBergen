Component specifications

TransformComponent

	Position (optional)
		att: x (float)
		att: y (float)
		att: z (float)

	Orientation (optional)
		att: yaw (float)
		att: pitch (float)
		att: roll (float)

MeshComponent
	Model (mandatory)
		att: format (const char*)
		text: path (const char*)

	ShaderProgram (optional)
		text: ShaderProgram (const char*)