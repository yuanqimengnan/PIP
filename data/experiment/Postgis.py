import psycopg2
import sys
import os
import tqdm
import re

pgisCon = psycopg2.connect(database='postgres', user='postgres',password='postgres', host='127.0.0.1', port=5432)


def prepare(polygonPath):
    pgisCursor = pgisCon.cursor()
    polygonfilename = os.path.basename(polygonPath)
    polygonfilename,_ = os.path.splitext(polygonfilename)
    drop_polygons_table_sql = f"drop table IF EXISTS  {polygonfilename};"
    pgisCursor.execute(drop_polygons_table_sql)
    create_polygons_table_sql = f"create table IF NOT EXISTS  {polygonfilename}(id integer PRIMARY KEY, polys geometry(Polygon));"
    pgisCursor.execute(create_polygons_table_sql)
    with open(polygonPath, 'r') as fp:
        num_polygon = int(fp.readline())
        for idx_polygon in tqdm.tqdm(range(num_polygon)):
            _ = fp.readline()
            line = fp.readline().split()
            polyid = int(line[0])
            num_vertex = int(line[1])
            i = 0
            PolygonValueSQL = "insert into {} values({},ST_GeomFromText('POLYGON(({}))'));"
            PolygonDATA = ""
            vertex_x = ""
            vertex_y = ""
            while i < num_vertex:
                vertex = fp.readline().split()
                if i == 0 :
                    vertex_x = vertex[0]
                    vertex_y = vertex[1]
                if(len(vertex) != 2) :
                    print("ERR:%s" % vertex)
                    exit(-1)
                PolygonDATA += ("%s %s,") % (vertex[0], vertex[1])
                i = i + 1
            PolygonDATA += ("%s %s,") % (vertex_x, vertex_y) 
            PolygonValueSQL = PolygonValueSQL.format(polygonfilename,polyid,PolygonDATA[:-1])
            #print(PolygonValueSQL)
            pgisCursor.execute(PolygonValueSQL)
    create_idx_sql =  f"create index idx_{polygonfilename} on {polygonfilename} using GIST(polys);"
    pgisCursor.execute(create_idx_sql)
    pgisCursor.close()
    pgisCon.commit()
    pgisCon.close()
    
def query(polygonPath, pointPath,experimentPath):
    pgisCursor = pgisCon.cursor()
    polygonfilename = os.path.basename(polygonPath)
    polygonfilename,_ = os.path.splitext(polygonfilename)
    
    # for points
    PointDATA = ""
    with open(pointPath, 'r') as fp:
        pointid = 0
        for line in fp:
            point = line.strip().split()
            x = point[0]
            y = point[1]
            PointDATA += "(%s, ST_GeomFromText('POINT(%s %s)'))," % (pointid, x, y)
            pointid = pointid + 1
    PointVauleSQL = "VALUES" + PointDATA[:-1]
    QuerySQL = f"EXPLAIN ANALYZE SELECT point.id AS point_id, polygon.id AS polygon_id FROM ({PointVauleSQL}) AS point(id, point) LEFT JOIN {polygonfilename} polygon on ST_Within(point.point, polygon.polys);"
    #print(QuerySQL)
    
    pgisCursor.execute(QuerySQL)
    
    results = pgisCursor.fetchall()
    planning_time = float(-1.0)
    execution_time = float(-1.0)
    for result in results:
        result_str = result[0]
        # extract planning time
        planning_match = re.search(r'Planning Time: ([\d.]+) ms', result_str)
        if planning_match:
            planning_time = float(planning_match.group(1))
        # extract execution time
        execution_match = re.search(r'Execution Time: ([\d.]+) ms', result_str)
        if execution_match:
            execution_time = float(execution_match.group(1))
    
    total_time = planning_time + execution_time
    
    pgisCursor.close()
    pgisCon.commit()
    pgisCon.close()
    
    print(total_time)
    with open(experimentPath, 'a') as ex:
        ex.write(str(total_time))
        ex.write("\n")
 
 
flag = sys.argv[1]
polygonPath = sys.argv[2]
if flag == 'prepare':
    prepare(polygonPath);
elif flag == 'query':
    pointPath = sys.argv[3]
    experimentPath = sys.argv[4]
    query(polygonPath, pointPath,experimentPath)
else:
    print("need a correct flag")
