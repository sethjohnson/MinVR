/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and 
maintained collaboratively by the University of Minnesota's Interactive 
Visualization Lab and the Brown University Visualization Research Lab.

File: MinVR/MVRCore/include/MVRCore/CameraOffAxis.H

Original Author(s) of this File: 
	Daniel Keefe, 2004, Brown University (originally VRG3D/ProjectionVRCamera.H)
	
Author(s) of Significant Updates/Modifications to the File:
	Bret Jackson, 2013, University of Minnesota (adapted to MinVR and glm)

-----------------------------------------------------------------------------------
Copyright (c) 2008-2015 Regents of the University of Minnesota and Brown University
Copyright (c) 2004-2008 Brown University
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of the University of Minnesota, Brown University, nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
================================================================================ */


#ifndef CAMERAOFFAXIS_H
#define CAMERAOFFAXIS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "MVRCore/AbstractCamera.H"
#include "MVRCore/StringUtils.H"

namespace MinVR {

/*! @Brief An offaxis VR camera
 *
 *  This derived class represents an offaxis view frustrum.
 *  It should be used head tracking in front of a flat display tile or in a VR CAVE.
 */
class CameraOffAxis : public AbstractCamera
{
public:
	CameraOffAxis(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 botLeft, glm::dvec3 botRight,
				  glm::dmat4 initialHeadFrame, double interOcularDistance, double nearClipDist, double farClipDist, bool isCoreProfile);

	virtual ~CameraOffAxis();

	/*! @brief Updates the camera's current head position.
	*
	*  This method is called by the MVREngine to update the current head position.
	*  It also recalculates the current offaxis projection matrix based on the head position and modelview matrix.
	*
	*  @param[in] The new head position.
	*/
	virtual void updateHeadTrackingFrame(glm::dmat4 newHeadFrame);

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices as if the camera is centered
	*  between the eyes. This is used for mono rendering.
	*/
	virtual void applyProjectionAndCameraMatrices();

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices as if the camera
	*  is located at the left eye. This is used for stereoscopic rendering.
	*/
	virtual void applyProjectionAndCameraMatricesForLeftEye();

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices as if the camera
	*  is located at the right eye. This is used for stereoscopic rendering.
	*/
	virtual void applyProjectionAndCameraMatricesForRightEye();

	/*! @brief Returns the last applied projection matrix.
	*/
	virtual glm::dmat4 getLastAppliedProjectionMatrix();

	/*! @brief Returns the last applied Model matrix.
	*/
	virtual glm::dmat4 getLastAppliedModelMatrix();

	/*! @brief Returns the last applied View matrix
	*/
	virtual glm::dmat4 getLastAppliedViewMatrix();


	/*! @brief Sets the object to world matrix.
	*
	*  This method sets the transformation between the current object (model) space and world (room)
	*  coordinate system. It is used when setting the opengl modelview matrix.
	*/
	virtual void setObjectToWorldMatrix(glm::dmat4 obj2World);

	/*! @brief Gets the object to world matrix.
		*
		*  This method gets the transformation between the current object (model) space and world (room)
		*  coordinate system. It is used when setting the opengl modelview matrix.
		*
		*/
	virtual glm::dmat4 getObjectToWorldMatrix();

	/*! @brief Gets the current location of the left eye.
	*
	*  Based on the current head position and interocular distance, this returns the left eye position
	*  
	*  @return A coordinate frame representing the translation and rotation for the left eye position.
	*/
	glm::dmat4 getLeftEyeFrame();

	/*! @brief Gets the current location of the right eye.
	*
	*  Based on the current head position and interocular distance, this returns the right eye position
	*  
	*  @return A coordinate frame representing the translation and rotation for the right eye position.
	*/
	glm::dmat4 getRightEyeFrame();

	/*! @brief Gets the vector from the head position to the center of the display tile
	*
	*  @return A vector from the current head position to the center of the display tile
	*/
	glm::dvec3 getLookVector();

	/*! @brief Gets the position of the display tile
	*
	*  @return An xyz position of the display tile
	*/
	glm::dvec3 getTopLeft() const;

	/*! @brief Gets the position of the display tile
	*
	*  @return An xyz position of the display tile
	*/
	glm::dvec3 getTopRight() const;

	/*! @brief Gets the position of the display tile
	*
	*  @return An xyz position of the display tile
	*/
	glm::dvec3 getBottomLeft() const;

	/*! @brief Gets the position of the display tile
	*
	*  @return An xyz position of the display tile
	*/
	glm::dvec3 getBottomRight() const;

protected:

	glm::dvec3 _topLeft;
	glm::dvec3 _topRight;
	glm::dvec3 _botLeft;
	glm::dvec3 _botRight;
	glm::dmat4 _headFrame;
	double _iod;
	double _nearClip;
	double _farClip;
	double _halfWidth;
	double _halfHeight;
	glm::dmat4 _room2tile;

	glm::dmat4 _projection;
	glm::dmat4 _projectionLeft;
	glm::dmat4 _projectionRight;
	glm::dmat4 _view;
	glm::dmat4 _viewLeft;
	glm::dmat4 _viewRight;
	glm::dmat4 _object2World;

	glm::dmat4 _currentViewMatrix;
	glm::dmat4 _currentProjMatrix;

	bool _isCoreProfile;

	virtual void applyProjectionAndCameraMatrices(const glm::dmat4& projectionMat, const glm::dmat4& viewMat);
	glm::dmat4 invertYMat();
	glm::dmat4 perspectiveProjection(double left, double right, double bottom, double top, double nearval, double farval, float upDirection = -1.0);

};

} //end namespace
#endif
