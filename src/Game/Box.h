#pragma once

#include "TinyMath3D.h"

namespace Internal {

	/** Stores rotation vectors used in the intersection tests,
	to avoid recalculating them each time. */
	class RotationState
	{
	public:
		RotationState(const Matrix3D& transform);
		Vector3D N[3];
		Matrix3D t;
	};

	/** AABB class, with support for testing against OBBs. */
	class Box
	{
	public:
		/** Default constructor */
		Box() {}
		/** Construct from scalar corner position and size */
		Box(float x, float y, float z, float sx, float sy, float sz)
			: m_Pos(x, y, z), m_Size(sx, sy, sz),
			m_Center(x + 0.5f * sx, y + 0.5f * sy, z + 0.5f * sz) {}
		/** Construct from corner position and size */
		Box(const Vector3D& pos, const Vector3D& size)
			: m_Pos(pos), m_Size(size), m_Center(pos + 0.5f * size) {}
		/** Copy constructor */
		Box(const Box& b) : m_Pos(b.m_Pos), m_Size(b.m_Size), m_Center(b.m_Center) {}
		virtual ~Box() {}
		/** Returns the box's position */
		const Vector3D& getPosition() const { return m_Pos; }
		/** Returns the sizes of the box's edges */
		const Vector3D& getSize() const { return m_Size; }
		/** Returns the center position of the box */
		const Vector3D& getCenter() const { return m_Center; }
		/** Returns the volume of the box */
		float getVolume() const { return m_Size.x * m_Size.y * m_Size.z; }
		/** Ray intersection */
		bool intersect(const Vector3D& O, const Vector3D& D) const;
		/** Line segment intersection */
		bool intersect(const Vector3D& O, const Vector3D& D, float segmax) const;
		/** Sphere intersection */
		bool intersect(const Vector3D& O, float radius) const;
		/** Point in box */
		bool intersect(const Vector3D& p) const;
		/** Aligned box intersection */
		bool intersect(const Box& b);
		/** Oriented box intersection. */
		bool intersect(const Box& b, RotationState& rs) const;

		/** Position of box corner */
		Vector3D m_Pos;
		/** Size of box box edges */
		Vector3D m_Size;
		/** Position of box center.  m_Pos+0.5f*m_Size;  */
		Vector3D m_Center;
	};

	/** A single triangle in the model */
	class Triangle
	{
	public:
		/** Default constructor */
		Triangle() {}
		/** Constructor to build a triangle from 3 points */
		Triangle(const Vector3D& _1, const Vector3D& _2, const Vector3D& _3);
		/** Tests for intersection with another triangle. */
		bool intersect(const Triangle& t) const;
		/** Tests for intersection with a ray (O origin, D direction)
		Returns true if collision occured.
		Outputs collision point in cp
		Outputs the distance from the origin to the collision point in tparm
		This distance is relative to the magnitude of D
		Allows testing against a finite segment, by specifying
		the maximum length of the ray in segmax
		This length is also relative to the magnitude of D
		*/
		bool intersect(const Vector3D& O, const Vector3D& D, Vector3D& cp,
			float& tparm, float segmax) const;
		/** Test for intersection with a sphere (O origin)
		Returns true if collision occured.
		Outputs collision point in cp
		*/
		bool intersect(const Vector3D& O, float radius, Vector3D* cp) const;

		void copyCoords(float array[9]) const;

		Vector3D v1, v2, v3;
		Vector3D center;
	};

	class BoxedTriangle;

	/** Base class for hierarchy tree nodes. */
	class BoxTreeNode : public Box
	{
	public:
		/** Default constructor */
		BoxTreeNode() : Box() {}
		/** Constructor for a box from position and size */
		BoxTreeNode(const Vector3D& pos, const Vector3D& size)
			: Box(pos, size) {}
		/** Returns true if the node is a leaf node. */
		virtual bool isLeaf() const = 0;
		/** Returns the number of sons this node has */
		virtual int getSonsNumber() const = 0;
		/** Returns a son node, by index */
		virtual const BoxTreeNode* getSon(int which) const = 0;
		BoxTreeNode* getSon(int which)
		{
			return const_cast<BoxTreeNode*>(const_cast<BoxTreeNode*>(this)->getSon(which));
		}
		/** Returns the number of triangles in this node.
		Only non-zero for leaf nodes. */
		virtual std::size_t getTrianglesNumber() const = 0;
		/** Returns the boxed triangle contained in this node
		by its index
		*/
		virtual const BoxedTriangle* getTriangle(std::size_t which) const = 0;
		BoxedTriangle* getTriangle(std::size_t which)
		{
			return const_cast<BoxedTriangle*>(const_cast<BoxTreeNode*>(this)->getTriangle(which));
		}
	};

	/** Inner node, containing other nodes. */
	class BoxTreeInnerNode : public BoxTreeNode
	{
	public:
		BoxTreeInnerNode(const Vector3D& pos, const Vector3D& size, int logdepth)
			: BoxTreeNode(pos, size),
			m_First(nullptr), m_OwnFirst(true),
			m_Second(nullptr), m_OwnSecond(true),
			m_logdepth(logdepth) {}
		~BoxTreeInnerNode()
		{
			if (m_OwnFirst)  delete m_First;
			if (m_OwnSecond) delete m_Second;
		}

		virtual bool isLeaf() const { return false; }
		/** Create the sons that will divide this box */
		int  createSons(const Vector3D& center);
		/** Recalculate the bounds of this box to fully contain
		all of its triangles
		*/
		void recalcBounds(Vector3D& center);
		/** Recursively divide this box */
		int  divide(int p_depth);

		int getSonsNumber() const
		{
			int n = 0;
			if (m_First != nullptr) n++;
			if (m_Second != nullptr) n++;
			return n;
		}

		std::size_t getTrianglesNumber() const;
		const BoxedTriangle* getTriangle(std::size_t which) const;

		const BoxTreeNode* getSon(int which) const
		{
			if (which == 0) return m_First;
			if (which == 1) return m_Second;
			return nullptr;
		}

		BoxTreeNode* m_First;
		bool                        m_OwnFirst;
		BoxTreeNode* m_Second;
		bool                        m_OwnSecond;
		int                         m_logdepth;
		std::vector<BoxedTriangle*> m_Boxes;
	};

	/** Leaf node, containing 1 triangle. */
	class BoxedTriangle : public BoxTreeNode, public Triangle
	{
	public:
		BoxedTriangle(const Vector3D& _1, const Vector3D& _2, const Vector3D& _3);
		virtual bool isLeaf() const { return true; }
		int getSonsNumber() const { return 0; }
		const BoxTreeNode* getSon(int /*which*/) const { return nullptr; }
		std::size_t getTrianglesNumber() const { return 1; }
		const BoxedTriangle* getTriangle(std::size_t which) const
		{
			if (which == 0) return this;
			return nullptr;
		}

	};

} // namespace Internal