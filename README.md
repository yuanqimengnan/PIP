# PIP
For GPUPlanCache

## Parameter
--polyPath your_polygons_path

--pointPath your_point_path

--outputResult

--experimentPath your_experiment_result_path

--nIter number_of_iterations

Note: If you do not specify the parameter "--outputResult", the experimental data will not be output to the experimentPath. 

## Description of polygons_file and points_file
### polygons_file
****************
Example:

2

1

0 3

0 0

0.5 0

0.5 0.5

1

1 4

0.5 0.5

1 0.5

1 1

0.5 1

****************
Polygon files must be provided strictly in the following format：

- The first line is the total number of polygons.
- Starting from the second line, describe each polygon.

  -- Firstly, using the number 1 to divide different polygons.
  
  -- Then, provide the ID of the polygon and the number of vertices. Note that the ID of the polygon needs to increase sequentially starting from 0.
  
  -- Next, provide the horizontal and vertical coordinates of each vertex in each row. Note that the size of each coordinate is between 0 and 1.

### points_file
****************
Example:

0.6 0.6

0.5 0.5

****************

Provide the horizontal and vertical coordinates of a series of points, ranging from 0 to 1.  
