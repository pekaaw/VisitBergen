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

	Scaling (optional)			% the text scale are applied first to all x,y,z
								% then, if attributes are given, each component will be
								% independantly changed. Default is 1 in all dimensions.
		text: scale (float)
		att: x (float)
		att: y (float)
		att: z (float)

MeshComponent
	Model (mandatory)
		att: format (const char*)
		text: path (const char*)

	ShaderProgram (optional)
		text: ShaderProgram (const char*)

	Normalize (optional)		% default true
		text: do normalization (bool)

	AverageNormals (optional)	% default false
		text: do averaging (bool)


SkyBoxComponent
	Model (mandatory)
		att: format (const char*)
		text: path (const char*)

	ShaderProgram (optional)
		text ShaderProgram (const char*)

	SkyBoxSides
		NegativeX (image),
		PositiveX (image),
		NegativeY (image),
		PositiveY (image),
		NegativeZ (image),
		PositiveZ (image)
			att: path (const char*)
