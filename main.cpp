#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

//main data struct to save locations
struct Data {
    double lat;
    double lon;
    std::string device;
    Data(double lat,double lon,std::string device)
    {
        this->lat=lat;
        this->lon=lon;
        this->device=std::move(device);
    }

};
//final distance and bearing data
struct Final {
    double distance;
    double bearing;
    std::string device;

    Final(double dis,double bear,std::string device)
    {
        this->distance=dis;
        this->bearing=bear;
        this->device=std::move(device);
    }

};
//sort distance
struct sort_distance
{
    inline bool operator() (const Final& struct1, const Final& struct2)
    {
        return (struct1.distance < struct2.distance);
    }
};

//2 vectors to store data
std::vector<Data> locations;
std::vector<Final> nearest;

//convert to rad
double radians(double val){
    return val * (M_PI / 180.0);
}

//convert to deg
double degrees(double val){
    return val *  (180.0/M_PI);
}

//Haversine  distance
double distance(double lat1, double lon1,double lat2, double lon2)
{
    double rad = 6371;
    double converted_lat1 = radians(lat1);
    double converted_lat2 = radians(lat2);
    double dLat = radians(lat2 - lat1);
    double dLon = radians(lon2 - lon1);
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(converted_lat1) * cos(converted_lat2);
    double c = 2 * asin(sqrt(a));
    return rad * c;
}




//A -> node location
//B -> node need help
double bearing(double latA, double lonA,double latB, double lonB)
{
    double theta1 = radians(latA);
    double theta2 = radians(latB);
    double delta2 = radians(lonB-lonA);
    double y = sin(delta2) * cos(theta2);
    double x = cos(theta1)*sin(theta2) - sin(theta1)*cos(theta2)*cos(delta2);
    double beta = atan2(y,x);
    double dt = degrees(beta)+360;
    dt = fmod(dt,360);
    return dt;
}

//collect and sort data
void collect(double lat,double lon,std::string name)
{
    //first element must be the node with SOS
    locations.emplace_back(Data(lat,lon,std::move(name)));

    if(locations.size()==3)
    {
        for(unsigned int i = 1; i < locations.size(); i++)
        {

            if(locations[i].lat==locations[0].lat&&locations[i].lon==locations[0].lon){
                nearest.emplace_back(Final(0,0,locations[i].device));
            }
            else
            {
                nearest.emplace_back(Final(distance(locations[i].lat,locations[i].lon,locations[0].lat,locations[0].lon),bearing(locations[i].lat,locations[i].lon,locations[0].lat,locations[0].lon),locations[i].device));
            }

        }

    }

    if(nearest.size()==2)
    {
        std::sort(nearest.begin(), nearest.end(), sort_distance());

        for(const auto& i:nearest)
        {
            std::cout<<i.distance<<std::endl;
            std::cout<<i.device<<std::endl;

        }
    }
}


//main method
int main() {

    collect(7.0937,79.9952,"A");
    collect(7.0937,79.9952,"B");
    collect(7.2401,80.1257,"Cap");

    return 0;
}


