//
//  ofxMeshSaver.h
//  RGBDepth_Playground
//
//  Created by Kamen Dimitrov on 12/19/12.
//
//

#pragma once

#include "ofMain.h"

class ofxMeshSaver {
public:
	static void saveMeshPoints(string fn, ofMesh &m) {
		
		// saving points
		
	
		ofFile f;
		f.open(fn, ofFile::WriteOnly, false);
		
				
		for(int i = 0; i < m.getNumVertices(); i++) {
			ofVec3f p1 = m.getVertex(i);
			addVertexToFile(f,p1);

		}
		
		f.close();
		ofLog() << "Saving " << m.getNumVertices() << " points at " << fn;

	}
	
	static void saveMeshIndices(string fn, ofMesh &m) {
		
		// saving points
		
		
		ofFile f;
		f.open(fn, ofFile::WriteOnly, false);
		
		int getNumTriangles = m.getNumIndices() / 3;
		
		for(int i = 0; i < getNumTriangles; i++) {
			int index1 = m.getIndex(i*3);
			int index2 = m.getIndex(i*3+1);
			int index3 = m.getIndex(i*3+2);
			
			addVertexToFile(f, ofVec3f(index1,index2,index3));
			
		}
		
		f.close();
		ofLog() << "Saving " << getNumTriangles << " triangles at " << fn;
		
	}
	
	static void addVertexToFile(ofFile &f, ofVec3f v) {
		std::stringstream stream;
		
		stream << v.x << "," << v.y << "," << v.z << endl;
		f << stream.str();
	}
};
