"""
Programming Exercise 1
Fysikopoulos Vissarion (mpla)
In the current file there are the functions that we need for
a triangulation of a simple polygon as well as a test of a 
triangulation. The code is, hopefully, well commented.
"""
import CGAL
import cgalvisual
import math 

from CGAL import *
from cgalvisual import *

# general computational geomerty functions
def ccw(i,j,k):
    return orientation(i,j,k) == CGAL.Kernel.Sign.LARGER

def collinear(i,j,k):
    return orientation(i,j,k) == CGAL.Kernel.Sign.EQUAL

def cw(i,j,k):
    return orientation(i,j,k) == CGAL.Kernel.Sign.SMALLER

def ccwon(i,j,k):
    return ccw(i,j,k) or collinear(i,j,k)

def between(i,j,k):
    if not collinear(i,j,k):
        return False
    else:
        s = Segment_2(i,j)
    return s.has_on(k)

# functions about lists
def shift_left(list):
	"""
	Returns the list shifted left (cyclic)
	"""
	return list[1:]+list[:1]

def shift_right(list):
	"""
	Returns the list shifted left (cyclic)
	"""
	return list[-1:]+list[:-1]

def is_in(k, list):
	"""
	Returns if k is in list
	"""
	for j in list:
		if j == k:
			return True
	return False
	
def min_index(list):
	"""
	Returns the index of the minimum in 
	a list
	"""
	min_index = 0
	n = len(list)
	for i in range(n):
		if min(list[i],list[min_index]) == list[i]:
			min_index = i
	return min_index
	
# functions about how it looks like
def points_label(points):
	"""
	Labels n points with numbers from 0 to n-1 
	"""
	k=0
	for i in points:
		i.label = `k`
		k += 1

# functions about simple polygon triangulation
def is_simple(points):
	"""
	Returns if the polygon given by the points is simple
	"""
	n = len(points)
	for i in range(n):
		for j in range(i+1,n):
			# if 2 not neighbour segments intersect		
			if intersection(points[i],points[(i+1)%n],points[j%n],points[(j+1)%n]) \
			   and (i+1)%n != j%n and i != (j+1)%n:
				 return False
	return True
	
def is_cw(points):
	"""
	Takes the n points of a simple polygon
	and returns if they are cw oriented
	time complexity O(n)
	"""
	n = len(points)
	min = min_index(points)
	return cw(points[min-1], points[min], points[(min+1)%n])
		
def colinear(i1,j1,i2,j2):
	"""
	Returns if the segments (i1,j1) (i2,j2) 
	are	colinear
	"""
	return orientation(i1,j1,i2) \
	       == CGAL.Kernel.Sign.EQUAL and \
		   orientation(i1,j1,j2) \
		   == CGAL.Kernel.Sign.EQUAL

def intersectionExtended(i1,j1,i2,j2):
	"""
	Returns if the segments (i1,j1) (i2,j2)
	would intersect if (i1,j1) extented far
	enough
	"""
	return orientation(i1,j1,i2) \
        != orientation(i1,j1,j2) \
		or colinear(i1,j1,i2,j2)

def colinearIntersection(i1,j1,i2,j2):
	"""
	Returns if the segments (i1,j1) intersect
	if (i1,j1) are colinear r
	"""
	return between(i1,j1,i2) or between(i1,j1,j2)  

def intersection(i1,j1,i2,j2):
	"""
	Returns if the segments (i1,j1) intersect 
	"""
	if  colinear(i1,j1,i2,j2):
		return colinearIntersection(i1,j1,i2,j2)
	else:
		return intersectionExtended(i1,j1,i2,j2) and intersectionExtended(i2,j2,i1,j1)  

def diagonalie(i, j, points):
	"""
	Returns if the segment from the point i to j
	do not intersect with other segments;
	that is an inner or an outer diagonial 
	O(n) time complexity
	"""
	n = len(points)
	# if (i,j) is a segment return False
	if i == j or math.fabs(i-j) == 1 or math.fabs(i-j) == n-1:
		return False
	for k in range(n):
		if intersection(points[i],points[j],points[k],points[(k+1)%n]) \
		   and k != i and k != j and (k+1)%n != i and (k+1)%n != j:
			return False
	return True

def ear(i, points):
	"""
	Returns if the i-th of the points shapes an ear 
	"""
	n = len(points)
	return cw(points[i-1],points[i],points[(i+1)%n]) \
	    == is_cw(points)

def incone(i, j, points):
	"""
	Returns if the j-th of the points is inside the
	cone of the i-th point in O(1) time complexity
	"""
	n = len(points)
	if ear(i, points):
		return cw(points[i-1],points[i],points[j]) \
		   and cw(points[j],points[i],points[(i+1)%n])
	else:
		return not(cw(points[i-1],points[j],points[i]) \
		           and cw(points[j],points[(i+1)%n],points[i]))

def diagonal(i, j, points):
	"""
	Returns if the segment from the point i to j
	is an inner diagonial in O(n) time complexity
	"""
	return incone(i,j,points) and diagonalie(i,j,points)

def triangulate(points):
	"""
	Return a triangulation of a simple polygon with n edges
	in O(n^3) time complexity
	"""
	diags=[]
	while len(points)>3:
		n=len(points)
		for i in range(n):
			i1=(i+1)%n
			i2=(i+2)%n
			if diagonal(i,i2,points):
				diags.append(VSegment_2(points[i],points[i2]))
				del(points[i1])
				break
	return diags

def triangulate2(points, segments):
	"""
	Return a triangulation of a simple polygon with n edges
	in O(n^2) time complexity. All the used functions run in 
	O(n) inside a for loop of O(n) complexity.
	"""
	n = len(points)
	diags=[]
	
	# the right and left neighbours of the points 
	left_points = shift_right(range(n))
	right_points = shift_left(range(n))
	
	# the stack with the ears
	eartips = filter(lambda i: diagonal(i-1,(i+1)%n,points),range(n))
	
	while eartips != []:
		# get an ear from the queue to cut
		eartip = eartips.pop(0)
		
		# the diagonal is the segment (ear_left,ear_right)
		ear_left = left_points[eartip]
		ear_right = right_points[eartip]
		print "current ear: ",eartip," left:",ear_left," right",ear_right
		diags.append(VSegment_2(points[ear_left],points[ear_right]))
		
		# color when cut an ear
		diags[len(diags)-1].color = visual.color.red
		points[eartip].color = visual.color.yellow
		segments[eartip].color = visual.color.yellow
		segments[eartip-1].color = visual.color.yellow
		
		# update the neighbours of ear_left, ear_right
		left_points[ear_right] = ear_left
		right_points[ear_left] = ear_right
		
		# compute properties for ear_left, ear_right [O(n)] 
		left_was_eartip = is_in(ear_left,eartips)
		left_is_eartip = diagonal(left_points[ear_left],right_points[ear_left],points)
		right_was_eartip = is_in(ear_right,eartips)
		right_is_eartip = diagonal(left_points[ear_right],right_points[ear_right],points)
		
		# update the eartip property for ear_left, ear_right
		if (not left_was_eartip) and left_is_eartip: 
			   eartips.append(ear_left)
		elif left_was_eartip and (not left_is_eartip):
				 eartips.remove(ear_left)
		if (not right_was_eartip) and right_is_eartip:
			   eartips.append(ear_right)
		elif right_was_eartip and (not right_is_eartip):
				 eartips.remove(ear_right)
		
		print "eartips: ",eartips
		mouseClick()
	return diags

#########################
# testing the functions #
#########################

points, segments = getPolygon()

# if the polygon is not cw we make it
if not is_cw(points):
	points.reverse()
	segments.reverse() 			

# put labels to points
points_label(points)

# fast triangualation if the polygon is simple
if not is_simple(points):
	print "The polygon is not simple"
else:
	P=points[:]
	visdiags=[]
	for diag in triangulate2(P,segments):
		d = VSegment_2(diag.start(),diag.end())
		d.color = visual.color.red
		visdiags.append(d)
