#include "trojanmap.h"
#include<map>
#include <unordered_map>
#include<limits>
#include<bits/stdc++.h>
#include <algorithm>

//-----------------------------------------------------
// TODO: Student should implement the following:
//-----------------------------------------------------
/**
 * GetLat: Get the latitude of a Node given its id. If id does not exist, return -1.
 * 
 * @param  {std::string} id : location id
 * @return {double}         : latitude
 */
double TrojanMap::GetLat(const std::string& id) {
  if(data.find(id)!=data.end())
  {
    return -1;
  }
  else
  {
    return data[id].lat;
    
  }
}

/**
 * GetLon: Get the longitude of a Node given its id. If id does not exist, return -1.
 * 
 * @param  {std::string} id : location id
 * @return {double}         : longitude
 */
double TrojanMap::GetLon(const std::string& id) { 
  if(data.find(id)!=data.end())
  {
    return -1;
  }
  else
  {
    return data[id].lon;
  }
}

/**
 * GetName: Get the name of a Node given its id. If id does not exist, return "NULL".
 * 
 * @param  {std::string} id : location id
 * @return {std::string}    : name
 */
std::string TrojanMap::GetName(const std::string& id) { 
    if(data.find(id)!=data.end())
  {
    return "";
  }
  else
  {
    return data[id].name;
  }
}

/**
 * GetNeighborIDs: Get the neighbor ids of a Node. If id does not exist, return an empty vector.
 * 
 * @param  {std::string} id            : location id
 * @return {std::vector<std::string>}  : neighbor ids
 */
std::vector<std::string> TrojanMap::GetNeighborIDs(const std::string& id) {
    if(data.find(id)!=data.end())
  {
    return {};
  }
  else
  {
    return data[id].neighbors;
  }
}

/**
 * GetID: Given a location name, return the id. 
 * If the node does not exist, return an empty string. 
 *
 * @param  {std::string} name          : location name
 * @return {int}  : id
 */
std::string TrojanMap::GetID(const std::string& name) {
for (auto str : data)
  {
    if (name == str.second.name)
    {
      return str.second.id;
    }
  }
  return "";
}
/**
 * GetPosition: Given a location name, return the position. If id does not exist, return (-1, -1).
 *
 * @param  {std::string} name          : location name
 * @return {std::pair<double,double>}  : (lat, lon)
 */
std::pair<double, double> TrojanMap::GetPosition(std::string name) {
  for (auto str : data)
  {
    if (name == str.second.name)
    {
      std::pair<double, double> results(str.second.lat, str.second.lon);
      return results;
    }
  }
  std::pair<double, double> no_results(-1, -1);
  return no_results;
}


/**
 * CalculateEditDistance: Calculate edit distance between two location names
 * 
 */
int TrojanMap::CalculateEditDistance(std::string a, std::string b){
  int m = a.size();
  int n = b.size();
  int D[2][m+1];
  for(int i=0;i<=m;i++)
    D[0][i] = i;
  for(int i=1;i<=n;i++)
  {
    for(int j=0;j<=m;j++)
    {
      if(j==0)
      D[i%2][j] = i; 
    else if(a[j-1] == b[i-1]){
      D[i%2][j] = D[(i-1)%2][j-1];
    }
    else{
      D[i%2][j]=1 + std::min(D[(i-1)%2][j],std::min(D[i%2][j-1],D[(i-1)%2][j-1]));
      }
    }
  }
  return D[n%2][m];
}

/**
 * FindClosestName: Given a location name, return the name with smallest edit distance.
 *
 * @param  {std::string} name          : location name
 * @return {std::string} tmp           : similar name
 */
std::string TrojanMap::FindClosestName(std::string name) {
  std::map<std::string, int> memo;
  for (auto str:data)
  {
    
    std::string data_name = str.second.name;
    if (memo.count(data_name) < 1)
    {
      memo[data_name] = TrojanMap::CalculateEditDistance(name,data_name);
    }
    
  }

  std::pair<std::string, int> min_name ("Target",10000);
  for (auto element:memo)
  {
    if (element.second < min_name.second)
    {
      min_name.first = element.first;
      min_name.second = element.second;
    }
  }
  return min_name.first;
}


/**
 * Autocomplete: Given a parital name return all the possible locations with
 * partial name as the prefix. The function should be case-insensitive.
 *
 * @param  {std::string} name          : partial name
 * @return {std::vector<std::string>}  : a vector of full names
 */
std::vector<std::string> TrojanMap::Autocomplete(std::string name){
  std::vector<std::string> results;
  name.erase(remove(name.begin(), name.end(), ' '), name.end());
  for(auto str : data){
    std::string loc_name = str.second.name;
    std::string lower="";
    lower.resize(loc_name.size());
    std::transform(loc_name.begin(),loc_name.end(),lower.begin(),::tolower);
    std::string lowername="";
    lowername.resize(name.size());
    std::transform(name.begin(),name.end(),lowername.begin(),::tolower);
    if(lower.find(lowername)==0){
      results.push_back(loc_name);
    }
  }
  return results;
}

/**
 * CalculateDistance: Get the distance between 2 nodes. 
 * 
 * @param  {std::string} a  : a_id
 * @param  {std::string} b  : b_id
 * @return {double}  : distance in mile
 */
double TrojanMap::CalculateDistance(const std::string &a_id, const std::string &b_id) {
  // Do not change this function
  Node a = data[a_id];
  Node b = data[b_id];
  double dlon = (b.lon - a.lon) * M_PI / 180.0;
  double dlat = (b.lat - a.lat) * M_PI / 180.0;
  double p = pow(sin(dlat / 2),2.0) + cos(a.lat * M_PI / 180.0) * cos(b.lat * M_PI / 180.0) * pow(sin(dlon / 2),2.0);
  double c = 2 * asin(std::min(1.0,sqrt(p)));
  return c * 3961;
}

/**
 * CalculatePathLength: Calculates the total path length for the locations inside the vector.
 * 
 * @param  {std::vector<std::string>} path : path
 * @return {double}                        : path length
 */
double TrojanMap::CalculatePathLength(const std::vector<std::string> &path) {
  // Do not change this function
  double sum = 0;
  for (int i = 0;i < int(path.size())-1; i++) {
    sum += CalculateDistance(path[i], path[i+1]);
  }
  return sum;
}

/**
 * CalculateShortestPath_Dijkstra: Given 2 locations, return the shortest path which is a
 * list of id. Hint: Use priority queue.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Dijkstra(
    std::string location1_name, std::string location2_name) {
    std::vector<std::string> path;
    std::string start = GetID(location1_name);
    std::string end = GetID(location2_name);
    if(start =="" || end==""){

    }
    else{
    std::priority_queue <std::pair<double,std::string>,std::vector<std::pair<double,std::string>>,std::greater<std::pair<double,std::string>>> minim_heap;                 //Priority queue to implement Heap with <distance,node ID>
    std::unordered_map <std::string,double> dist;                 //Unordered Map to store the distance from root to current node
    for(auto nodes:data){
      dist[nodes.second.id] = DBL_MAX;
    }
    std::unordered_map <std::string,std::string> predecessor;   //Unordered map to store the predecessor of the node
    std::unordered_map <std::string,bool> visited;          //Unord Map to keep track if node is visited
    for(auto nodes:data){
      visited[nodes.second.id] = false;
    }
    dist[start] = 0;
    minim_heap.push(std::make_pair(dist[start],start));
    while(!minim_heap.empty()){
      std::string current = minim_heap.top().second;
      minim_heap.pop();
      if(current!=end){
        if(CalculateDistance(current,start)>dist[current]){
          continue;
        }
        else if(visited[current]){
          continue;
        }
        else{
          visited[current] = true;
          for(auto neighbour : data[current].neighbors){
            double new_dist = dist[current] + CalculateDistance(current,neighbour);
             if(dist[neighbour]>new_dist){
               dist[neighbour] = new_dist;
               predecessor[neighbour] = current;   
               minim_heap.push(std::make_pair(dist[neighbour],neighbour));          
          }
          }
        }
      }
      else{
      visited[end] = true;
      break;
      }
    }
    if(!visited[end])
     return path;
    for(auto node = end; node!= start; node = predecessor[node])
    {
      path.push_back(node);
    }
    path.push_back(start);
    std::reverse(path.begin(),path.end());
    }
  return path;

}
/**
 * CalculateShortestPath_Bellman_Ford: Given 2 locations, return the shortest path which is a
 * list of id. Hint: Do the early termination when there is no change on distance.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Bellman_Ford(
    std::string location1_name, std::string location2_name){
    std::vector<std::string> path;
    std::string start = GetID(location1_name);
    std::string end = GetID(location2_name);
    std::unordered_map <std::string,double> dist;       //Unordered Map to store the distance from root to current node
    int f = 0;
    for(auto nodes:data){
      dist[nodes.second.id] = DBL_MAX;
    }
    std::unordered_map <std::string,std::string> predecessor; //Unordered map to store the predecessor of the node
    dist[start] = 0;
    if(start!=end){
      for (int i = 0; i < data.size()-1; i++){
        for (auto pair: data){
            std::string current = pair.second.id;
            for(auto neighbour : pair.second.neighbors){
                  double new_dist = dist[current] + CalculateDistance(current,neighbour);
                  if(dist[neighbour]>new_dist){
                    dist[neighbour] = new_dist;
                    predecessor[neighbour] = current;                    
                    f++;
                  }
            }
        }
        if(f==0){
              break;
            }
            f = 0;
      }
    }
    if(dist[end]==DBL_MAX)
      return path;
    for(auto node = end; node!= start; node = predecessor[node])
    {
      path.push_back(node);
    }
    path.push_back(start);
    std::reverse(path.begin(),path.end());
    return path;
}

/**
 * Travelling salesman problem: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of total distance and the all the progress to get final path
 */
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan_Brute_force(
                                    std::vector<std::string> location_ids) {
        std::pair<double, std::vector<std::vector<std::string>>> records;
        if(location_ids.size()<=1){
          std::vector<std::vector<std::string>> path;
        return std::make_pair(0,path);
        }

      double min_length = DBL_MAX;
      std::vector <std::string> temp;
      std::vector <std::string> cur_path;
      std::map<std::string,bool> visited;
      temp.assign(location_ids.begin()+1,location_ids.end());
      std::sort(temp.begin(),temp.end());
      do{
        cur_path.push_back(location_ids[0]);
        for(auto id:temp){
          cur_path.push_back(id);
        }
        cur_path.push_back(location_ids[0]);
      double cur_path_length = CalculatePathLength(cur_path);
      cur_path.clear();
      if(cur_path_length<min_length){
        min_length = cur_path_length;
        records.first = min_length;
        std::vector <std::string> path ;
        path.push_back(location_ids[0]);
        for(auto id: temp){
          path.push_back(id);
        }
        path.push_back(location_ids[0]);
        records.second.push_back(path);
      }
      }
      while(std::next_permutation(temp.begin(),temp.end()));
      return records;
}

   std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan_Backtracking(
                                    std::vector<std::string> location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> records;
    
  double min_distance = INT_MAX;
  std::vector<std::string> curr_path;
 
  curr_path.push_back(location_ids[0]);

  
  std::unordered_map<std::string, double> distance;

  
  distance[location_ids[0]] = 0;

  
  for (auto &id: location_ids) {
    distance[id] = CalculateDistance(location_ids[0], id);
  }

  double curr_distance = 0;

  std::vector<std::string> optimal_path;

  Backtracking_Helper(location_ids[0], distance, location_ids, location_ids[0], curr_distance, 
               curr_path, min_distance, records, optimal_path);

  records.first = min_distance;
  records.second.push_back(optimal_path);
  return records;
}

void TrojanMap::Backtracking_Helper(std::string start, std::unordered_map<std::string, double> distance,
                             std::vector<std::string> location_ids,
                             std::string curr_node, double curr_distance,
                             std::vector<std::string> &curr_path,
                             double &min_distance,  std::pair<double, std::vector<std::vector<std::string>>> &records,
                             std::vector<std::string> &optimal_path) {
  // If the current node is the last node, update the minimum distance
  if (curr_path.size() == distance.size()) {
    if (curr_distance + distance[curr_node] < min_distance) {
      min_distance = curr_distance + distance[curr_node];
      optimal_path = curr_path;
      optimal_path.push_back(location_ids[0]);
    }
    curr_path.push_back(location_ids[0]);
    records.second.push_back(curr_path);
    curr_path.pop_back();
    return;
  }
  // If the current distance is larger than the minimum distance, return
  if (curr_distance >= min_distance) {
    return;
  }

  // Else evaluate all the children of the current node
  for (int i = 0; i < location_ids.size(); i++) {
    if (std::find(curr_path.begin(), curr_path.end(), location_ids[i]) == curr_path.end()) {
      // Add the current node to the path
      curr_path.push_back(location_ids[i]);

      // Call backtracking recursively
      Backtracking_Helper(start, distance, location_ids, location_ids[i], 
                   curr_distance + CalculateDistance(curr_node, location_ids[i]),
                   curr_path, min_distance, records, optimal_path);
      // Remove the current node from the path
      curr_path.pop_back();
    }
  }
}

std::vector<std::string> TrojanMap::twoOptSwap(const std::vector<std::string> &path, int i, int k) {
  std::vector<std::string> swapped(path);
  std::reverse(swapped.begin() + i, swapped.begin() + k + 1);
  return swapped;
}

std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan_2opt(
      std::vector<std::string> location_ids){
  std::pair<double, std::vector<std::vector<std::string>>> records;
    
std::vector<std::string> cur_route = location_ids;
  cur_route.push_back(location_ids[0]);
  bool flag = true;
  int nums = location_ids.size();
  // Do swap until there is no improvement
  while(flag){
    start_again:
    flag = false;
    double cur_path = CalculatePathLength(cur_route);
    for(int i = 1; i <= nums - 2; i++){
      for(int k = i + 1; k <= nums - 1; k++){
        auto new_route = twoOptSwap(cur_route, i, k);
        double new_path = CalculatePathLength(new_route);
        if(new_path < cur_path){
          cur_route = new_route;
          cur_path = new_path;
          records.first = cur_path;
          records.second.push_back(cur_route);
          flag = true;
          goto start_again;
        }
      }
    }
  }
  return records;
}

/**
 * Given CSV filename, it read and parse locations data from CSV file,
 * and return locations vector for topological sort problem.
 *
 * @param  {std::string} locations_filename     : locations_filename
 * @return {std::vector<std::string>}           : locations 
 */
std::vector<std::string> TrojanMap::ReadLocationsFromCSVFile(std::string locations_filename){
  std::vector<std::string> location_names_from_csv;

  std::fstream myFile;
  myFile.open(locations_filename, std::ios::in);
  std::string line;
  getline(myFile,line);
  while (std::getline(myFile, line)) 
  {
    line.erase(std::remove(line.begin(),line.end(),','), line.end());
    if(line!="" )
      location_names_from_csv.push_back(line);
  }
  myFile.close();

  return location_names_from_csv;

}

/**
 * Given CSV filenames, it read and parse dependencise data from CSV file,
 * and return dependencies vector for topological sort problem.
 *
 * @param  {std::string} dependencies_filename     : dependencies_filename
 * @return {std::vector<std::vector<std::string>>} : dependencies
 */
std::vector<std::vector<std::string>> TrojanMap::ReadDependenciesFromCSVFile(std::string dependencies_filename){
  std::vector<std::vector<std::string>> dependencies_from_csv;

  std::fstream myFile;
  myFile.open(dependencies_filename, std::ios::in);
  std::string line;

  std::getline(myFile, line);
  while (std::getline(myFile, line)) {
    //Creating 2 strings for the two locations
    std::string loc1 , loc2;
    auto loc = line.find(',');
    if(loc==0 || loc==line.size()-1)
    {
      continue;
    } 
    loc1 = line.substr(0,loc);
    loc2 = line.substr(loc+1);
    loc2.erase(std::remove(loc2.begin(),loc2.end(),','), loc2.end());
    dependencies_from_csv.push_back({loc1,loc2});
  }
  myFile.close();

  return dependencies_from_csv;
}

/**
 * DeliveringTrojan: Given a vector of location names, it should return a sorting of nodes
 * that satisfies the given dependencies. If there is no way to do it, return a empty vector.
 *
 * @param  {std::vector<std::string>} locations                     : locations
 * @param  {std::vector<std::vector<std::string>>} dependencies     : prerequisites
 * @return {std::vector<std::string>} results                       : results
 */
std::vector<std::string> TrojanMap::DeliveringTrojan(std::vector<std::string> &locations,
                                                     std::vector<std::vector<std::string>> &dependencies){
  std::vector<std::string> result;
  std::unordered_map<std::string, std::vector<std::string>> adjacency; //adjacency list
  std::map<std::string, bool> visited;

  for(auto location: locations) {
    std::vector<std::string> temp;
    adjacency[location] = temp;
  }

  for(auto dependency: dependencies) {
    adjacency[dependency[0]].push_back(dependency[1]);
  }

  for(auto location: locations)
  {
    visited[location]=false;
  }

  //DFS through each location
  for(auto location: locations)
  {
    if(!visited[location])
      DFS_Helper(location, visited, adjacency, result);
  }
  std::unique(result.begin(), result.end());
  std::reverse(result.begin(), result.end());
  return result;                                                     
}  

void TrojanMap::DFS_Helper(std::string location, std::map<std::string, bool>& visited, 
                          std::unordered_map<std::string, std::vector<std::string> >&adjacency, std::vector<std::string>& result)
{
  visited[location] = true; 
  for(const std::string neighbor: adjacency[location]) 
  {
    if(!visited[neighbor])
    {
      DFS_Helper(neighbor, visited, adjacency, result);
    }
  }
  result.push_back(location); 
}                                    

/**
 * inSquare: Give a id retunr whether it is in square or not.
 *
 * @param  {std::string} id            : location id
 * @param  {std::vector<double>} square: four vertexes of the square area
 * @return {bool}                      : in square or not
 */
bool TrojanMap::inSquare(std::string id, std::vector<double> &square) {
  std::vector<std::string> square_id = GetSubgraph(square);
  if(std::find(square_id.begin(), square_id.end(), id) != square_id.end()) {
    return true;
  } else {
    return false;
  }
}

/**
 * GetSubgraph: Give four vertexes of the square area, return a list of location ids in the squares
 *
 * @param  {std::vector<double>} square         : four vertexes of the square area
 * @return {std::vector<std::string>} subgraph  : list of location ids in the square
 */
std::vector<std::string> TrojanMap::GetSubgraph(std::vector<double> &square) {
  // include all the nodes in subgraph
  std::vector<std::string> subgraph;
  double ver1 = square[0];
  double ver2 = square[1];
  double ver3 = square[2];
  double ver4 = square[3];

  for(auto j = data.begin(); j != data.end(); j++){
    if((data[j->first].lon)>ver1 && (data[j->first].lon)<ver2){
      if((data[j->first].lat)<ver3 && (data[j->first].lat)>ver4){
        subgraph.push_back(data[j->first].id);
      }
    }
  }
  return subgraph;
}

/**
 * Cycle Detection: Given four points of the square-shape subgraph, return true if there
 * is a cycle path inside the square, false otherwise.
 * 
 * @param {std::vector<std::string>} subgraph: list of location ids in the square
 * @param {std::vector<double>} square: four vertexes of the square area
 * @return {bool}: whether there is a cycle or not
 */

bool TrojanMap::hasCycle(std::string current_id,std::unordered_map<std::string, bool> &visited, std::string parent_id){
  visited[current_id] = true;
  for(auto n:data[current_id].neighbors){
    if(visited.find(n) != visited.end()){ //to check if the neighbor is in the area
      if(visited[n] == false){
        if(hasCycle(n,visited,current_id)){
          return true;
        }
      }else if((n!=parent_id) && (visited[n]== true)){
          return true;
      }
    }
  }
  return false;
}

bool TrojanMap::CycleDetection(std::vector<std::string> &subgraph, std::vector<double> &square) {
  std::unordered_map<std::string, bool> visited;

  for(auto id_i:subgraph){
    visited[id_i] = false;
  }

  for(auto id_itr:subgraph){
    if(visited[id_itr] == false){
      if (hasCycle(id_itr,visited,"")){
        return true;
      }
    }
  }
  return false;
}


/**
 * FindNearby: Given a class name C, a location name L and a number r, 
 * find all locations in class C on the map near L with the range of r and return a vector of string ids
 * 
 * @param {std::string} className: the name of the class
 * @param {std::string} locationName: the name of the location
 * @param {int} r: search radius
 * @param {int} k: search numbers
 * @return {std::vector<std::string>}: location name that meets the requirements
 */
std::vector<std::string> TrojanMap::FindNearby(std::string attributesName, std::string name, double r, int k) {
  std::vector<std::string> res;

  std::string current_id = GetID(name);
  std::priority_queue<std::pair<std::string,double>> locations;

  //Traversing the nodes
  for (auto node:data)
  {
    //Current node
    if (node.second.id == current_id)
    {
      continue;
    }

    //Only traversing those locations who possess the desired attribute
    if (node.second.attributes.count(attributesName) > 0)
    {
      //Calculating the sidtance of the location from the source
      double dist = CalculateDistance(node.second.id, current_id);
      //If place inside the radius, we continue. Else it is discarded. We also check the size of the locations list to make sure we have only the k closest locations
      if (dist <= r && (locations.size() < k || dist < locations.top().second))
      {
        if (locations.size() >= k)
        {
          locations.pop();
        }
        locations.push({node.second.id, dist});
      }
    }
  }

  while (locations.size() != 0)
  {
    auto loc = locations.top();
    res.push_back(loc.first);
    locations.pop();

  }
  std::reverse(res.begin(),res.end());
  return res;
}

/**
 * CreateGraphFromCSVFile: Read the map data from the csv file
 * 
 */
void TrojanMap::CreateGraphFromCSVFile() {
  // Do not change this function
  std::fstream fin;
  fin.open("src/lib/data.csv", std::ios::in);
  std::string line, word;

  getline(fin, line);
  while (getline(fin, line)) {
    std::stringstream s(line);

    Node n;
    int count = 0;
    while (getline(s, word, ',')) {
      word.erase(std::remove(word.begin(), word.end(), '\''), word.end());
      word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '{'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '}'), word.end());
      if (count == 0)
        n.id = word;
      else if (count == 1)
        n.lat = stod(word);
      else if (count == 2)
        n.lon = stod(word);
      else if (count == 3)
        n.name = word;
      else {
        word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
        if (isalpha(word[0]))
          n.attributes.insert(word);
        if (isdigit(word[0]))
          n.neighbors.push_back(word);
      }
      count++;
    }
    data[n.id] = n;
  }
  fin.close();
}
