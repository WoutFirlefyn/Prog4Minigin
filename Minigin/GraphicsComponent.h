#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------


//-----------------------------------------------------
// GraphicsComponent Class									
//-----------------------------------------------------
class GraphicsComponent final
{
public:
	GraphicsComponent();				// Constructor
	~GraphicsComponent();				// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	GraphicsComponent(const GraphicsComponent& other)					= delete;
	GraphicsComponent(GraphicsComponent&& other) noexcept				= delete;
	GraphicsComponent& operator=(const GraphicsComponent& other)		= delete;
	GraphicsComponent& operator=(GraphicsComponent&& other)	noexcept	= delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Update() const = 0;



private: 
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------

	
};

 
