// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    //Initializing the unordered map we need and other stuff
    placeID_names_map = {};

}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::place_count()
{
    //returns how many places there have been saved to the map
    int places = placeID_names_map.size();

    return places;

}

void Datastructures::clear_all()
{
    // Clears all the maps.
    placeID_names_map.clear();
    placeID_type_map.clear();
    placeID_coord_map.clear();

}

std::vector<PlaceID> Datastructures::all_places()
{

    std::vector<PlaceID>places_to_return;

    for (std::pair<PlaceID, Name> elem : placeID_names_map) {
        places_to_return.push_back(elem.first);
    }
    return places_to_return;

}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{

    //if given ID already exist, return false
    if (placeID_names_map.count(id) > 0) {
        return false;
    }

    placeID_names_map.insert({id, name});
    placeID_type_map.insert({id, type});
    placeID_coord_map.insert({id, xy});
    return true;

}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    //runs two iterators, maybe we worry about optimization later :/
    std::unordered_map<PlaceID, Name>::const_iterator name_iter = placeID_names_map.find(id);
    //if we can't find the name before we run out of map, return NO_NAME and NO_TYPE consts
    if (name_iter == placeID_names_map.end()) {
        return {NO_NAME, PlaceType::NO_TYPE};
    }
    std::unordered_map<PlaceID, PlaceType>::const_iterator place_iter = placeID_type_map.find(id);

    //otherwise we can return the name and the placetype
    return {name_iter->second, place_iter->second};

}

Coord Datastructures::get_place_coord(PlaceID id)
{
    //Iterator goes through placeID_coord_map, until it finds given ID and returns the coordinates
    std::unordered_map<PlaceID, Coord>::const_iterator coord_iter = placeID_coord_map.find(id);

    //if it can't find given ID before we run through the map, we return NO_COORD
    if (coord_iter == placeID_coord_map.end()) {
        return NO_COORD;
    }

    return coord_iter->second;

}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{

    //if we can find the ID before areaID_name map ends, it already exists and return false

    if (areaID_name_map.find(id) != areaID_name_map.end()) {
        return false;
    }
    //otherwise we can stick the information into our maps / maybe later into the struct
    areaID_name_map.insert({id, name});
    areaID_coord_map.insert({id, coords});

    return true;

}

Name Datastructures::get_area_name(AreaID id)
{
    //iterate the area_ID_name map and if we can't find the AreaID, return NO_NAME
    std::unordered_map<AreaID, Name>::const_iterator get_area_iter
            = areaID_name_map.find(id);

    if (get_area_iter == areaID_name_map.end()) {
        return NO_NAME;
    }
    //If we do find the AreaID, return the name
    return get_area_iter->second;

}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{

    //iterate the areaID_coord_map, if we can't find AreaID, return NO_NAME again
    std::unordered_map<AreaID, std::vector<Coord>>::const_iterator got_coords_iter
            = areaID_coord_map.find(id);
    if (got_coords_iter == areaID_coord_map.end()) {
        return {NO_COORD};
    }
    //business as usual here, return the name if we find the AreaID
    return got_coords_iter->second;

}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}


std::vector<PlaceID> Datastructures::places_alphabetically()
{


    std::vector<PlaceID>IDs_alphabetically;

    std::vector<std::pair<PlaceID, Name>>IDs_to_be_sorted;

    //copying the stuff we need from the map where it's located
    std::copy(placeID_names_map.begin(), placeID_names_map.end(),
              std::back_inserter(IDs_to_be_sorted));
    //sorting the vector using name_comp, a function I wrote below
    std::sort(IDs_to_be_sorted.begin(), IDs_to_be_sorted.end(), name_comp);

    //pushing only the ID's into the IDs_alphabetically-vector.
    for (std::vector<std::pair<PlaceID, Name>>::iterator it = IDs_to_be_sorted.begin();
         it != IDs_to_be_sorted.end(); ++it ) {
        IDs_alphabetically.push_back(it->first);

    }


    return IDs_alphabetically;

}

std::vector<PlaceID> Datastructures::places_coord_order()
{

    //ugly solution but best I can do right now
    std::vector<std::pair<PlaceID, Coord>> closest_to_origo;

    for (auto& iter : placeID_coord_map) {
        closest_to_origo.push_back(std::make_pair(iter.first, iter.second));
    }

    //sort the  vector using the coord_comp function I made lower down
    std::sort (closest_to_origo.begin(), closest_to_origo.end(), coord_comp);

    //
    std::vector<PlaceID> coord_ordered_ids;

    for (auto& iter2 : closest_to_origo) {
        coord_ordered_ids.push_back(iter2.first);
    }
    return coord_ordered_ids;

}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{


    //vector for the placenames we wanna return
    std::vector<PlaceID> placenames;

    //iterate through placeID_names map, push placenames into the vector if they match const& name
    for (std::unordered_map<PlaceID, Name>::iterator iter = placeID_names_map.begin();
         iter != placeID_names_map.end(); ++iter) {
        if (iter->second == name) {
            placenames.push_back(iter->first);
        }
    }

    return placenames;

}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{

    //vector for placeID's
    std::vector<PlaceID> placeIDs;

    //iterate through the map, push placeID's into the vector when they match PlaceType type
    for (std::unordered_map<PlaceID, PlaceType>::iterator iter
         = placeID_type_map.begin(); iter != placeID_type_map.end(); ++iter) {

        if (iter->second == type) {
            placeIDs.push_back(iter->first);
        }
    }
    return placeIDs;

}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    //if we count zero such ID's in the map, return false
    if (placeID_names_map.count(id) < 1) {
        return false;
    }
    //otherwise find said id, change it's name, return true
    auto find = placeID_names_map.find(id);
    find->second = newname;
    return true;

}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{

    //if we count zero such ID's in the map, return false
    if (placeID_coord_map.count(id) < 1) {
        return false;
    }
    //otherwise find said id, change it's coordinates, return true
    auto find = placeID_coord_map.find(id);
    find->second = newcoord;
    return true;

}

std::vector<AreaID> Datastructures::all_areas()
{

    //vector for our areas
    std::vector<AreaID> areas;

    //for-loop, push the areas in areaID_name_map in the vector we made
    std::cout << "all_areas" << std::endl;
    for (std::pair<AreaID, Name> elem : areaID_name_map) {
        areas.push_back(elem.first);
    }

    //return the vector
    return areas;

}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{


    //first we check if we can find the given ID's
    auto find_area = areaID_name_map.find(id);
    auto find_area_parent = areaID_name_map.find(parentid);
    //if we can't, do nothing, return false
    if (find_area == areaID_name_map.end() || find_area_parent == areaID_name_map.end()) {
        return false;
    }
    //then we check if the subarea is already part of an area
    auto check_subarea = areaID_subarea_map.find(id);
    //if yes, do nothing, return false
    if (check_subarea != areaID_subarea_map.end()) {
        return false;
    }
    //otherwise just put stuff in the subarea map
    areaID_subarea_map.insert({id, parentid});
    return true;

}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{


    //again first check if we can find given area
    auto find_subarea = areaID_name_map.find(id);
    //if not, return NO_AREA
    if (find_subarea == areaID_name_map.end()) {
        return {NO_AREA};
    }

    //if yes, we make a vector for our subarea ID's
    std::vector<AreaID>areaIDs;

    for (std::unordered_map<AreaID, AreaID>::iterator iter = areaID_subarea_map
         .begin(); iter != areaID_subarea_map.end(); ++iter) {
        auto find_area = areaID_subarea_map.find(id);
        if (find_area != areaID_subarea_map.end()) {
            areaIDs.push_back(find_area->second);
            id = find_area->second;
        }
    }
    return areaIDs;

}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::remove_place(PlaceID id)
{
    //just erases everything from all the maps

    areaID_name_map.erase(id);
    placeID_names_map.erase(id);
    placeID_type_map.erase(id);
    placeID_coord_map.erase(id);
    areaID_name_map.erase(id);
    areaID_subarea_map.erase(id);
    areaID_coord_map.erase(id);

    return true;

}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    // Replace this comment with your implementation


    return {NO_AREA};
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    // Replace this comment with your implementation
    return NO_AREA;
}

std::vector<WayID> Datastructures::all_ways()
{
    // Makes a vector for all the ways, puts all the ways in the vector, returns it.
    // Probably works now?

    std::vector <WayID> all_ways = {};
    for (std::vector<Way>::iterator iter = ways_vector.begin(); iter != ways_vector.end(); ++iter) {
        all_ways.push_back(iter->id);
    }

    return all_ways;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{   
    // Implementation assumes coords-vector only contains two coordinates.



    for (unsigned long i = 0; i < ways_vector.size(); ++i) {
        if (ways_vector.at(i).id == id) {
            return false;
        }
    }

    Way new_way;
    new_way.id = id;
    new_way.way_coords = coords;
    ways_vector.push_back(new_way);

    return true;
}

std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{
    // Replace this comment with your implementation
    return {{NO_WAY, NO_COORD}};
}

std::vector<Coord> Datastructures::get_way_coords(WayID id)
{
    // Finds a way with parameter WayID and returns it's coords.
    // IF they can't be found, returns NO_COORD.

    std::vector<Coord> get_way_coords_vect;

    //if we find a way with the given ID, copy it's way_coords to our return vector
    for (unsigned long i = 0; i < ways_vector.size(); ++i) {
        if (ways_vector.at(i).id == id) {
            get_way_coords_vect = ways_vector.at(i).way_coords;
            return get_way_coords_vect;
        }
    }
    //otherwise we just return the NO_COORD.
    return {NO_COORD};
}

void Datastructures::clear_ways()
{
    //clears the ways-vector.
    ways_vector.clear();
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_any(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

bool Datastructures::remove_way(WayID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

std::vector<std::tuple<Coord, WayID> > Datastructures::route_with_cycle(Coord fromxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY}};
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}

/*
 * A comparison function for calculating the distance of a coordinate
 * from origo. This was painful.
 */

bool Datastructures::coord_comp(std::pair<PlaceID, Coord> coordA, std::pair<PlaceID, Coord> coordB)
{

    //get the coordinates we need for comparison
    //calculate the euclidian distances from origo by using pythagoras
    //jesus christ
    double coordAXdouble = pow(coordA.second.x, 2);
    double coordAYdouble = pow(coordA.second.y, 2);
    double coordAdist = sqrt(coordAXdouble+coordAYdouble);
    double coordBXdouble = pow(coordB.second.x, 2);
    double coordBYdouble = pow(coordB.second.y, 2);
    double coordBdist = sqrt(coordBXdouble+coordBYdouble);


    //if distance is the same, we compare Y's, return smaller
    if (coordAdist == coordBdist) {
        if ( coordA.second.y == coordB.second.y) {
            return true;
        }
        else if (coordA.second.y < coordB.second.y) {
            return true;
        }
        else {
            return false;
        }
    }

    //else we return the smaller coordinate
    else {
        if (coordAdist < coordBdist) {
            return true;
        }
        else {
            return false;
        }
    }
}

/*
 * A comparison function to sort out alphabetic order.
 */

bool Datastructures::name_comp(std::pair<PlaceID, Name> nameA, std::pair<PlaceID, Name> nameB)
{
    return nameA.second < nameB.second;
}
