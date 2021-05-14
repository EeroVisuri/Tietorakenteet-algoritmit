// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <unordered_map>
#include <map>
#include <iostream>


// Types for IDs
using PlaceID = long long int;
using AreaID = long long int;
using Name = std::string;
using WayID = std::string;

// Return values for cases where required thing was not found
PlaceID const NO_PLACE = -1;
AreaID const NO_AREA = -1;
WayID const NO_WAY = "!!No way!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Enumeration for different place types
// !!Note since this is a C++11 "scoped enumeration", you'll have to refer to
// individual values as PlaceType::SHELTER etc.
enum class PlaceType { OTHER=0, FIREPIT, SHELTER, PARKING, PEAK, BAY, AREA, NO_TYPE };

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;



// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Calling .size() on a map is a constant time operation.
    int place_count();


    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear on size, because it goes through all the items in maps.
    void clear_all();


    // Estimate of performance: O(n)
    // Short rationale for estimate: Linear, because function has a for-loop, which executes n times.
    std::vector<PlaceID> all_places();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Goes through placeId_names_map, then inserts into 3 other maps.
    // Will optimize if I have time
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Has an iterator going through items
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Uses map's "find", which is logarithmic
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n(log(n)))
    // Short rationale for estimate: Sorting average case is n log n, so that's what we get.
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance: O(n(log(n)))
    // Short rationale for estimate: Sorting average case is n log n.
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance: O(n)
    // Short rationale for estimate: For-loop for n, push_back is constant time
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For-loop for n, push_back is constant time
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Find is logarithmic time taking maneuver.
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Find takes logarithmic time again
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Find takes logarithmic time, insertions are constant on average
    // worst case is linear in container size
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Again find from map takes logarithmic time.
    Name get_area_name(AreaID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Again find from map takes logarithmic time.
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For-loop iterates through n elements, push_back is constant time
    std::vector<AreaID> all_areas();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: Multiple finds takes log n time, inserts are constant
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: O(n(log(n)))
    // Short rationale for estimate: Log-time find in a for loop takes n log n time. This one's ugly.
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_place(PlaceID id);

    // Estimate of performance:
    // Short rationale for estimate:
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

    // Phase 2 operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: For-loop where we insert n-times is O(n).
    std::vector<WayID> all_ways();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Checks through ways_vector for ID, then adds it if missing.
    bool add_way(WayID id, std::vector<Coord> coords);

    // Estimate of performance: O(log^n)
    // Short rationale for estimate: Goes through ways_vector with for loop, has an inner for-loop.
    std::vector<std::pair<WayID, Coord>> ways_from(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Again a for-loop going through ways_vector.
    std::vector<Coord> get_way_coords(WayID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Clearing a vector depends on it's size.
    void clear_ways();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_any(Coord fromxy, Coord toxy);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_way(WayID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_least_crossroads(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID>> route_with_cycle(Coord fromxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_shortest_distance(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_ways();

private:
    // Add stuff needed for your class implementation here



    // A struct to store info about our places. Maybe we don't need this tho.
    struct Place {
        PlaceID id;
        AreaID areaId;
        Name name;
        WayID wayId;
        Coord coordinates;
        PlaceType type;
        CoordHash coordinateHashed;

    };
    //yeah ended up not using this at all maybe it would have been neater but


    struct Way_node {
        Coord position;
        int val;
        bool visited = false;
        struct Way_node *next;
        struct Way_node *prev;

    };


    //struct for edges in the graph
    struct Way {
    public:
        WayID id;
        std::vector<Coord>way_coords_vect;
        Distance waylength;
        Coord start;
        Coord end_coord;
        Way_node *startnode;
        Way_node *endnode;
        int start_ver;
        int end_ver;
        struct Way *next;
        struct Way *prev;



        Way() {

        }

        //parametized constructor, calculates its own waylength and start and end coords are saved.
        Way(WayID id1, std::vector<Coord>way_coords_vect1, std::vector<Way> ways_vector) {
            id = id1;
            waylength = 0;
            way_coords_vect = way_coords_vect1;
            if (way_coords_vect.size() == 1) {
                start = way_coords_vect[0];
                end_coord = way_coords_vect[0];
                waylength = 0;

            }else {
                for (unsigned long i = 0; i < way_coords_vect.size()-1; ++i) {
                    waylength = waylength+way_length(way_coords_vect[i], way_coords_vect[i+1]);
                }
            }
            start = way_coords_vect[0];
            end_coord = way_coords_vect.back();

            //save pointers for next and previous ways too
            for (unsigned long i = 0; i < ways_vector.size(); ++i) {
                if (ways_vector.at(i).start == this->end_coord) {
                    this->next = &ways_vector.at(i);
                }
                if (ways_vector.at(i).end_coord == this->start) {
                    this->prev =&ways_vector.at(i);
                }
            }

        }


    };
    //
    //

    class graph {
        Way_node* get_adjlist_node(int value, Distance weight, Way_node* head) {
            Way_node* new_node = new Way_node;
            new_node->val = value;
            new_node->next = head;
            new_node->val = weight;
            return new_node;
        }
        int nodes; //how many nodes in graph.

    public:
        Way_node **head; //adjacency list as array of ptrs
        //allocate new node
        graph(Way ways[], int n) {
            head = new Way_node*[n]();
            this->nodes = n;
            //construct directed graph, add edges
            for (int i = 0; i < n; ++i) {
                int start_ver = ways[i].start_ver;
                int end_ver = ways[i].end_ver;
                Distance weight = ways[i].waylength;
                Way_node *new_node = get_adjlist_node(end_ver, weight, head[start_ver]);
                //head pointer to new_node
                head[start_ver] = new_node;
            }
        }
        //destructor
        ~graph(){
            for (int i = 0; i < nodes; ++i) {
                delete [] head[i];
                delete [] head;
            }
        }
    };



    void make_graph(int edges, Datastructures::Way edgeslist[]);

    std::pair<WayID, int>get_shortest_way(std::vector<Way> ways);

    int get_node_index (Coord pos);

    bool make_node (Coord pos, Coord next);

    static std::string find_way (Coord pos, std::vector<Way> ways_vector);

    int find_way_distance (Coord pos);

    std::vector<std::tuple<Coord, WayID, Distance>> route(Coord fromxy, Coord toxy);



    static int way_length(Coord fromxy, Coord toxy);


    //std::unordered_map <PlaceID, Place> placeId_Places_map; maybe not this

    //a comparison function for sorting by coordinate distance from origo

    static bool coord_comp(std::pair<PlaceID, Coord> coordA, std::pair<PlaceID, Coord> coordB);

    //a comparison function for sorting alphabetically

    static bool name_comp(std::pair<PlaceID, Name> nameA, std::pair<PlaceID, Name> nameB);

    //ton of maps to save our data

    std::unordered_map <PlaceID, Name> placeID_names_map = {};

    std::unordered_map <PlaceID, PlaceType> placeID_type_map = {};

    std::unordered_map <PlaceID, Coord> placeID_coord_map = {};

    std::unordered_map <AreaID, Name> areaID_name_map = {};

    std::unordered_map <AreaID, AreaID> areaID_subarea_map = {};

    std::unordered_map <AreaID, std::vector<Coord>> areaID_coord_map = {};

    
    std::vector <Way> ways_vector = {};

    std::vector <Coord> vertexes = {};

    std::vector <Way_node> nodes_vector = {};

};

#endif // DATASTRUCTURES_HH
