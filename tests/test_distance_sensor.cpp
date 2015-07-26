#define CATCH_CONFIG_RUNNER
#include "../libs/catch.hpp"
#include "distance_sensor.h"
#include <SDL/SDL.h>
#include "robot.h"
#include "garden.h"


TEST_CASE("Distance sensor can sense distance"){
    std::vector<Drawable*> things_to_sense;
    double measurement1, measurement2;

    //Make our sensor host
    Robot* robot = new Robot();
    robot->set_position(0, 100);

    //Make our sensor
    Distance_Sensor *sensor1, *sensor2;
    double center_angle1 = 20;
    double center_angle2 = -20;

    double sensor_width = 60;

    sensor1 = new Distance_Sensor(robot, center_angle1, sensor_width);
    sensor2 = new Distance_Sensor(robot, center_angle2, sensor_width);

    //Make the thing to sense
    Garden* garden = new Garden();
    garden->set_position(700, 100);
    things_to_sense.push_back(garden);

    measurement1 = sensor1->sense(things_to_sense);
    measurement2 = sensor2->sense(things_to_sense);

    REQUIRE(measurement1 == measurement2);
    REQUIRE(measurement1 > 0);

    //Should be the limit of sensing with both eyes
    robot->rotate(-10);

    measurement1 = sensor1->sense(things_to_sense);
    measurement2 = sensor2->sense(things_to_sense);
    REQUIRE(measurement1 == measurement2);
    REQUIRE(measurement1 > 0);

    //This should remove garden from sensor 2
    robot->rotate(-1);

    measurement1 = sensor1->sense(things_to_sense);
    measurement2 = sensor2->sense(things_to_sense);
    REQUIRE(measurement1 > 0);
    REQUIRE(measurement2 == 0);

    //Remove the garden completely from view
    robot->rotate(-40);
    measurement1 = sensor1->sense(things_to_sense);
    measurement2 = sensor2->sense(things_to_sense);
    REQUIRE(measurement1 == 0);
    REQUIRE(measurement2 == 0);

    //Back to 0deg. Test a movement
    robot->rotate(51);
    measurement1 = sensor1->sense(things_to_sense);
    robot->move(50);
    measurement2 = sensor1->sense(things_to_sense);
    REQUIRE(measurement1 < measurement2);
    printf("measurement1: %f", measurement1);
    printf("measurement2: %f", measurement2);


}


TEST_CASE("Distance Sensor can test if angle is in range"){
    Distance_Sensor *sensor;
    Robot* robot;

    robot = new Robot();
    //These values do not matter
    sensor = new Distance_Sensor(robot,0, 0);

    //Accute negative
    REQUIRE(sensor->angle_in_range(0,-30,30) == true);
    REQUIRE(sensor->angle_in_range(-40,-30,30) == false);
    //Accute positive
    REQUIRE(sensor->angle_in_range(20,10,30) == true);
    REQUIRE(sensor->angle_in_range(40,10,30) == false);
    //Its inclusive
    REQUIRE(sensor->angle_in_range(20,20,30) == true);

    //Obtuse angles
    REQUIRE(sensor->angle_in_range(180,30,330) == true);
    REQUIRE(sensor->angle_in_range(180,30,-30) == true);
    REQUIRE(sensor->angle_in_range(0,30,-30) == false);



}

int main( int argc, char* argv[] )
{
      
    int result = Catch::Session().run( argc, argv );
     
          // global clean-up...
    
    return result;
}

