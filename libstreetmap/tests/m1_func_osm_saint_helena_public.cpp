
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

#include <UnitTest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(osm_queries_public_saint_helena) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                loadMap("/cad2/ece297s/public/maps/saint-helena.streets.bin");
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test setup: loadMap threw an exceptinon !!!!" << std::endl;
                throw; // re-throw exceptinon
            }
        }
    
        ~BaseMapFixture() {
            //Clean-up
            try {
                closeMap();
            } catch (const std::exception& e) {
                std::cout << "!!!! BaseMapFixture test teardown: closeMap threw an exceptinon. what(): " << e.what() << " !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test teardown: closeMap threw an exceptinon !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            }
        }
    };


    struct MapFixture : BaseMapFixture {};

    TEST_FIXTURE(MapFixture, osm_node) {
        LatLon expected;

        expected = LatLon(-16.02420425415039062, -5.74945163726806641);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(3805562528)));

        expected = LatLon(-16.02070045471191406, -5.73693037033081055);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(249207659)));

        expected = LatLon(-16.01844215393066406, -5.73422956466674805);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(319312098)));

        expected = LatLon(-16.01766777038574219, -5.73352956771850586);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(319312058)));

        expected = LatLon(-16.01676940917968750, -5.73196506500244141);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(319332208)));

        expected = LatLon(-16.01541709899902344, -5.73091554641723633);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6269521280)));

        expected = LatLon(-16.01409912109375000, -5.72933197021484375);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6269521357)));

        expected = LatLon(-16.01385307312011719, -5.72862529754638672);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6269522346)));

        expected = LatLon(-16.01339340209960938, -5.72701501846313477);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6313366651)));

        expected = LatLon(-16.00820732116699219, -5.72686719894409180);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6269522043)));

        expected = LatLon(-15.97560310363769531, -5.72667980194091797);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6116679344)));

        expected = LatLon(-15.95558834075927734, -5.72582674026489258);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(5371719678)));

        expected = LatLon(-15.95217990875244141, -5.72560977935791016);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(1371843729)));

        expected = LatLon(-15.94900226593017578, -5.72551870346069336);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(5599404969)));

        expected = LatLon(-15.94831657409667969, -5.72431182861328125);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6313365587)));

        expected = LatLon(-15.94580554962158203, -5.72419595718383789);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(6313365411)));

        expected = LatLon(-15.94482135772705078, -5.72408914566040039);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(4147614458)));

        expected = LatLon(-15.94424533843994141, -5.72381973266601562);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(3795429109)));

        expected = LatLon(-15.94335556030273438, -5.72337388992309570);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(4147614266)));

        expected = LatLon(-15.94259548187255859, -5.72318124771118164);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(4147614181)));

        expected = LatLon(-15.94258403778076172, -5.72274684906005859);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(4147614180)));

        expected = LatLon(-15.94257926940917969, -5.72263717651367188);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(3795436340)));

        expected = LatLon(-15.92981433868408203, -5.72240161895751953);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(1371847362)));

        expected = LatLon(-15.92914676666259766, -5.72092723846435547);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(333643542)));

        expected = LatLon(-15.92898368835449219, -5.71576929092407227);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(5374163377)));

        expected = LatLon(-15.92879772186279297, -5.71511363983154297);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(5374163272)));

        expected = LatLon(-15.92858886718750000, -5.71458864212036133);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(2153070538)));

        expected = LatLon(-15.92662811279296875, -5.69204521179199219);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(2559934402)));

        expected = LatLon(-15.91892910003662109, -5.66933107376098633);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(3865374611)));

        expected = LatLon(-15.91873741149902344, -5.65185737609863281);
        ECE297_CHECK_EQUAL(expected, findLatLonOfOSMNode(OSMID(319373626)));

    } //osm_node

} //osm_queries_public_saint_helena

