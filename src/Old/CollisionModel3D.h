#pragma once

// Fork claudette


class ModelCollisionTest;
class RayCollisionTest;
class SphereCollisionTest;

/*! Collision Model, will represent the mesh to be tested for collisions
*
*  It has to be notified of all triangles, via addTriangle(). \n
*  After all triangles are added, a call to finalize() will process the
*  information and prepare for collision tests. \n
*  Call modelCollision(), rayCollision(), sphereCollision() to check for a
*  collision
*
*  \section CollisionModel3d_model_setup Model setup
*
*  For each mesh, create a collision model with:
*  \code
*      // Static collision model :
*      CollisionModel3D* model = new CollisionModel3D;
*  \endcode
*  (Shared meshes can use the same model)
*
*  Add all the triangles the mesh has to the collision model with:
*  \code
*      model->addTriangle(vertex1, vertex2, vertex3);
*  \endcode
*
*  Then call:
*  \code
*      model->finalize();
*  \endcode
*/
class CollisionModel3D
{
public:
	/*! Various model types */
	enum ModelType {
		/*! The model is dynamic ie. can move */
		DynamicModel,
		/*! The model is static ie. does not move and certain calculations can
		* be done every time its transform changes instead of every collision
		* test */
		StaticModel
	};

	CollisionModel3D(ModelType type = StaticModel);
	~CollisionModel3D();

	/*! Optional: Optimization for construction speed.
	*
	*  If you know the number of triangles */
	void setTriangleCount(std::size_t count);

	/*! Add a triangle to the mesh */
	void addTriangle(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3);
	/*! \overload */
	void addTriangle(const float v1[3], const float v2[3], const float v3[3]);

	/*! All triangles have been added, process model */
	void finalize();

	/*! The current affine matrix for the model
	*
	*  Matrix format is column-major order :
	*  \code
	*    a11 a12 a13 Tx
	*    a21 a22 a23 Ty
	*    a31 a32 a33 Tz
	*     0   0   0   1
	*
	*  Array: { a11, a21, a31, 0, a12, a22, a32, 0, a13, a23, a33, 0,  Tx,  Ty,  Tz, 1 }
	*  \endcode
	*
	*  \note Transformations must not contain scaling
	*/
	void setTransform(const float m[16]);

	/*! Checks for collision with another model (do not mix model types)
	*
	*  \param test the test object for mesh/mesh collisions
	*  \param maxProcessingTime determines the maximum time in milliseconds
	*         to check for collision.  If a rejection is not found by that
	*         time, the function will return true. \n
	*
	*  \warning ModelCollisionTest::accuracyDepth() is not yet supported
	*
	*  \sa ModelCollisionTest::maxProcessingTimedOut()
	*/
	bool modelCollision(ModelCollisionTest* test, int maxProcessingTime = 0) const;

	/*! Returns \c true if the ray given in world space coordinates intersects
	*  with the object */
	bool rayCollision(RayCollisionTest* test) const;

	/*! Returns \c true if the given sphere collides with the model */
	bool sphereCollision(SphereCollisionTest* test) const;

private:
	class Private;
	Private* const d;
};

/*! Inconsistency exception
*
*  Exception will be thrown if the model is inconsistent.
*  Examples:
*     \li Checking for collisions before calling finalize()
*     \li Trying to add triangles after calling finalize()
*/
class Inconsistency {};



//////////////////////////////////////////////
// Utility Functions
//////////////////////////////////////////////

/*! Checks for intersection between a ray and a sphere
*
*  \param point will contain point of intersection, if one is found. */
bool SphereRayCollision(
	const float sphereCenter[3],
	float sphereRadius,
	const float rayOrigin[3],
	const float rayDirection[3],
	float point[3]);

/*! Checks for intersection between 2 spheres */
bool SphereSphereCollision(
	const float c1[3], float r1,
	const float c2[3], float r2,
	float point[3]);