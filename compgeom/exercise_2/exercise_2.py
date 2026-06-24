"""
Programming Exercise 2
Fisikopoulos Vissarion (mpla)
In the current file there are the functions that we need for
moving a point inserted by the user and a voronoi vertex inside 
a constructed voronoi diagram. Also one can use the sample data 
sets for the construction of extreme and general voronoi diagrams.
"""

import CGAL
import cgalvisual


from CGAL import *
from cgalvisual import *
from random import *
from visual import *

# visual window settings
scene.title = "Voronoi Diagram"
scene.width = 1430
scene.height = 1200
scene.x = 0
scene.y = 0

def getRandomPoints(num):	
	PointList = []
	for i in range(num):
		point = VPoint_2(uniform(0,100), uniform(0,100))
		PointList.append(point)
	return PointList

def getFilePoints(filename):	
	#filename = raw_input('Enter file name: ')
	PointList = []
	fobj = open(filename, 'r')
	for line in fobj:
		point = VPoint_2(float(line.strip().split()[0]),\
		                 float(line.strip().split()[1]))
		PointList.append(point)
	fobj.close()
	return PointList

def buildVoronoi(points):
	dt = Delaunay_triangulation_2()
	for currentPoint in points:
		dt.insert(currentPoint)
			
	voronoiSegments = []
	voronoiRays = []
	voronoiLines = []
	for e in dt.edges:
		if dt.dual(e).__class__ == CGAL.Kernel.Ray_2:
			v = VRay_2(dt.dual(e).start(),dt.dual(e).direction())
			voronoiRays.append(v)
		elif dt.dual(e).__class__ == CGAL.Kernel.Segment_2:
			v = VSegment_2(dt.dual(e).start(),dt.dual(e).end())
			voronoiSegments.append(v)
		elif dt.dual(e).__class__ == CGAL.Kernel.Line_2:
			v = VLine_2(dt.dual(e))
			voronoiLines.append(v)
	return dt, voronoiSegments, voronoiRays, voronoiLines

def colorCell(point, dt):
	edges = []
	vertex = dt.nearest_vertex(point)
	
	degree =  vertex.degree()
	edge_circulator = dt.incident_edges(vertex)
	for i in range(degree):
		edge = edge_circulator.next()
		if not(dt.is_infinite(edge)):
			if dt.dual(edge).__class__ == CGAL.Kernel.Ray_2:
				near = VRay_2(dt.dual(edge).start(),dt.dual(edge).direction())
			elif dt.dual(edge).__class__ == CGAL.Kernel.Segment_2:
				near = VSegment_2(dt.dual(edge))
			elif dt.dual(edge).__class__ == CGAL.Kernel.Line_2:
				near = VLine_2(dt.dual(edge))
			near.color = visual.color.red
			edges.append(near)
	return edges

def movePoint(dt, velosity):
	s = getVSegment_2()
	s.visible = False
	p1 = s[0]
	p1.color = visual.color.green 
	
	p2 = s[1]
	p2.color = visual.color.green 
	
	x1, x2 = p1.cartesian(0), p2.cartesian(0) 
	y1, y2 = p1.cartesian(1), p2.cartesian(1)
	length = math.pow(x2-x1,2) + math.pow(y2-y1,2)
	for i in range(0, 100):
		p = VPoint_2(x1 + i*(x2-x1)/100, y1 + i*(y2-y1)/100)
		p.color = visual.color.green 
		cell_edges = colorCell(p,dt)
		time.sleep( math.sqrt(s.squared_length())/(100 * velosity))

#
# testing ...		
#
while True:
	
	velocity = 100
	
	# Input data points
	# choose 1/4
	#
	
	# n lines only
	#points = getFilePoints("line.txt")
	
	# n rays only
	#points = getFilePoints("ray.txt")
	
	# 3 rays and segments only
	#points = getFilePoints("data.txt")
	
	# random points
	points = getRandomPoints(20)
	
	# user's input
	#points = getVisualPoints()
	
	
	[dt, voronoiSegments, voronoiRays, voronoiLines] = buildVoronoi(points)		
	
	movePoint(dt, velocity)
	
	
	# A dummy algorithm for
	# Cell moving reconstructing voronoi
	# time complexity is bad ... 
	# only in neighbour cells an update is needed
	
	# choose the last point
	p1 = points[-1]
	p1.color = visual.color.blue
	time.sleep(1)
	# move to a random direction
	p2 = VPoint_2(uniform(-20,20),uniform(-20,20))
	p1.visible = False
	p2.visible = False
	
	x1, x2 = p1.cartesian(0), p2.cartesian(0) 
	y1, y2 = p1.cartesian(1), p2.cartesian(1)
	length = math.sqrt(math.pow(x2-x1,2) + math.pow(y2-y1,2))
	for i in range(0, 100):
		p = VPoint_2(x1 + i*(x2-x1)/100, y1 + i*(y2-y1)/100)
		p.color = visual.color.blue 
		points[-1] = p
		[dt, voronoiSegments, voronoiRays, voronoiLines] = buildVoronoi(points)
		time.sleep( length/(100 * velocity))
	points[-1].color = visual.color.white
	
	mouseClick()
	dt.clear()
	clearScene()
