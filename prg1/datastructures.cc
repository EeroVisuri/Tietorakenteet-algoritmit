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

    //this whole thing is awful with 2 for loops, gotta optimize somehow
    std::vector<PlaceID>IDs_alphabetically;
    //this map is going to be alphabetically sorted due to std::less<Name>
    std::map<Name, PlaceID, std::less<Name>> alphabetic_order_map;

    //iterate the unordered map, insert stuff into the regular map
    std::unordered_map<PlaceID, Name>::iterator iter;
    for (iter = placeID_names_map.begin(); iter != placeID_names_map.end(); iter++) {
        alphabetic_order_map.insert(std::make_pair(iter->second, iter->first));
    }
    //put the stuff into the vector we're gonna return
    //todo: check if we can just put stuff into the vector in alphabetic order
    for (const auto& elem : alphabetic_order_map) {
        IDs_alphabetically.push_back(elem.second);
    }

    return IDs_alphabetically;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    std::vector<std::pair<PlaceID, Coord>> closest_to_origo;

    for (auto& iter : placeID_coord_map) {
        closest_to_origo.push_back(std::make_pair(iter.first, iter.second));
    }

    std::sort (closest_to_origo.begin(), closest_to_origo.end(), coord_comp);


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
    //for-loop is slow maybe optimize this?
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
    //for-loop again maybe look into it when optimizing?
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
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_AREA};
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::remove_place(PlaceID id)
{
    // Replace this comment with your implementation
    return false;
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

