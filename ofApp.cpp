#include "ofApp.h"
#include <iostream>
#include <fstream>
/**
 Author: Henry Lai
 Description: this program utilize functions of open framworks and read in obj files, it then displays the model in
 wireframe form. this program also supports drag and drop files into the program window.
 */

//--------------------------------------------------------------
void ofApp::setup(){
   cam.setDistance(30.0); //adjusts the initial camera distance
   cam.lookAt(glm::vec3(0, 0, 0)); //initial camera pointing position
   ofSetBackgroundColor(ofColor::black);

   // hard coded cube
   mesh.vertices.push_back(glm::vec3(1,1,-1));
   mesh.vertices.push_back(glm::vec3(1,-1,-1));
   mesh.vertices.push_back(glm::vec3(1,1,1));
   mesh.vertices.push_back(glm::vec3(1,-1,1));
   mesh.vertices.push_back(glm::vec3(-1,1,-1));
   mesh.vertices.push_back(glm::vec3(-1,-1,-1));
   mesh.vertices.push_back(glm::vec3(-1,1,1));
   mesh.vertices.push_back(glm::vec3(-1,-1,1));

   mesh.triangles.push_back(Triangle(4,2,0));
   mesh.triangles.push_back(Triangle(2,7,3));
   mesh.triangles.push_back(Triangle(6,5,7));
   mesh.triangles.push_back(Triangle(1,7,5));
   mesh.triangles.push_back(Triangle(0,3,1));
   mesh.triangles.push_back(Triangle(4,1,5));
   mesh.triangles.push_back(Triangle(4,6,2));
   mesh.triangles.push_back(Triangle(2,6,7));
   mesh.triangles.push_back(Triangle(6,4,5));
   mesh.triangles.push_back(Triangle(1,3,7));
   mesh.triangles.push_back(Triangle(0,2,3));
   mesh.triangles.push_back(Triangle(4,0,1));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
   cam.begin();
   //ofDrawGrid();
   //ofSetColor(0, 255, 0);

   //drawing axis
   ofSetColor(ofColor::green);
   ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 5.0, 0));
   ofSetColor(ofColor::red);
   ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(5.0, 0, 0));
   ofSetColor(ofColor::blue);
   ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 5.0));

   //looping through triangles to outpuut into draw methods.
   for (int i = 0; i < mesh.triangles.size(); i++){
      int x = mesh.triangles[i].vertInd[0];
      int y = mesh.triangles[i].vertInd[1];
      int z = mesh.triangles[i].vertInd[2];

      ofSetColor(ofColor::white);
      ofNoFill();
      ofDrawTriangle(mesh.vertices[x], mesh.vertices[y], mesh.vertices[z]);
   }

   for (int i = 0; i < mesh.vertices.size(); i++){
      ofDrawCircle(mesh.vertices[i], 0.1);
   }

   cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
   readFile(dragInfo.files[0]);
   cout << "file: " << dragInfo.files[0] << endl;
   cout << "contains " << mesh.vertices.size() << " vertices." << endl;
   cout << "contains " << mesh.triangles.size() << " faces." << endl;
   float vertSize = sizeof(float) * 3 * mesh.vertices.size();
   float triSize = sizeof(int) * 3 * mesh.triangles.size();
   if (((vertSize + triSize) / 1000) < 500){
      cout << "estimated file size: " << (vertSize + triSize) / 1000 << " KB" << endl;
   } else {
      cout << "estimated file size: " << (vertSize + triSize) / 1000000 << " MB" << endl;
   }
   draw();
}

//--------------------------------------------------------------
//method to read given file directory (.obj files) into mesh
void ofApp::readFile(string fileDir){
   ifstream file;
   file.open(fileDir.c_str());
   //internal edit to test model after shifted away from origin.
   static const float shiftX = 0, shiftY = 0, shiftZ = 0;

   //only does stuff if file is sucessfully opened
   if (file.is_open()) {
      //line stores entire row of string form txt
      //subStr stores seperated strings temperarily
      string line, subStr;
      //clear previous mesh
      mesh.vertices.clear();
      mesh.triangles.clear();

      cout << "file opened" << endl;

      while (getline(file, line)) {
         //skipping empty lines
         if(line != "")
         {
            stringstream str(line);
            vector<string> tokens;

            while (getline(str, subStr, ' ')) {
               tokens.push_back(subStr);
            }

            if (tokens[0] == "v") {
               float v1 = stof(tokens[1]);
               float v2 = stof(tokens[2]);
               float v3 = stof(tokens[3]);
               //shifts model according to set coordiates
               mesh.vertices.push_back(glm::vec3(v1 + shiftX, v2 + shiftY, v3 + shiftZ));

//               debugging tools for reading coordinate infos
//               cout << "x:" << tokens[1] << " -- y:" << tokens[2] << " -- z:" << tokens[3] << endl;

            } else if (tokens[0] == "f") {
               stringstream ss;

               int indx1 = tokens[1].find('/');
               int indx2 = tokens[2].find('/');
               int indx3 = tokens[3].find('/');

               if(indx1 < 0) indx1 = tokens[1].length();
               if(indx2 < 0) indx2 = tokens[2].length();
               if(indx3 < 0) indx3 = tokens[3].length();

               indx1 = stoi(tokens[1].substr(0, indx1));
               indx2 = stoi(tokens[2].substr(0, indx2));
               indx3 = stoi(tokens[3].substr(0, indx3));

               //triangle indexies are adjusted by 1 and pushed into mesh
               mesh.triangles.push_back(Triangle(indx1 - 1, indx2 - 1, indx3 - 1));
//               debugging tools for reading triangle infos
//               cout << "first: " << indx1
//                    << " second: " << indx2
//                    << " third: " << indx3 << endl;
            }
         }
      }

      //read in line by line and insert seperated values into mesh
//      while (getline(file, line)) {
//         cout << line.substr(0,2) << endl;
//         if (line.substr(0,2) == "v ") {
//            cout << "found v" << endl;
//         } else if (line.substr(0,2) == "f ") {
//            cout << "found f" << endl;
//         }
//      }

      file.close();

   } else {
      cout << "file not found" << endl;
   }
}
