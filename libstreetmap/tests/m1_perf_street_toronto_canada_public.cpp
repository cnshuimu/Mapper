
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

SUITE(street_queries_perf_public_toronto_canada) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                loadMap("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
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


    struct MapFixture : BaseMapFixture {
        MapFixture()
            : BaseMapFixture()
            , rng(3)
            , rand_intersection(0, getNumIntersections()-1)
            , rand_street(1, getNumStreets()-1) // Start from 1 to avoid getting id 0 (<unknown>)
            , rand_segment(0, getNumStreetSegments()-1)
            , rand_poi(0, getNumPointsOfInterest()-1)
            , rand_feature(0, getNumFeatures()-1)
            , rand_node(0, 7780698)
            , rand_way(0, 975994)
            , rand_relation(0, 7144)
            , rand_lat(43.479999542, 43.919982910)
            , rand_lon(-79.789985657, -79.000000000)
        { }

        std::minstd_rand rng;
        std::uniform_int_distribution<int> rand_intersection;
        std::uniform_int_distribution<int> rand_street;
        std::uniform_int_distribution<int> rand_segment;
        std::uniform_int_distribution<int> rand_poi;
        std::uniform_int_distribution<int> rand_feature;
        std::uniform_int_distribution<int> rand_node;
        std::uniform_int_distribution<int> rand_way;
        std::uniform_int_distribution<int> rand_relation;
        std::uniform_real_distribution<double> rand_lat;
        std::uniform_real_distribution<double> rand_lon;
    };

    TEST_FIXTURE(MapFixture, all_street_intersections_perf) {
        //Verify Functionality
        std::vector<IntersectionIdx> expected;

        expected = {2, 3, 10, 747, 748, 751, 752, 753, 754, 757, 758, 762, 763, 766, 767, 775, 776, 777, 779, 780, 1924, 1943, 7130, 9987, 9992, 9993, 10105, 10261, 10262, 10269, 10270, 10314, 10315, 13309, 13316, 13321, 14890, 14891, 14892, 14924, 14925, 15018, 15019, 15020, 16935, 16936, 19252, 20209, 24442, 24463, 24464, 29254, 29549, 29550, 29551, 29560, 39266, 39267, 58608, 58609, 58941, 58942, 58955, 73821, 73835, 73836, 73837, 73850, 74698, 74699, 89294, 89516, 89520, 95111, 110416};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(2)));

        expected = {1783, 5704, 52919, 97534, 97535, 97536, 97537};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(20584)));

        expected = {11483, 11486, 75620, 95988};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(20493)));

        expected = {16049, 93850};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(20298)));

        expected = {16144, 64389};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(14345)));

        expected = {16963, 16972, 16973, 16974, 16975, 72491, 108589};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(4069)));

        expected = {18199, 18200, 18201};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(4419)));

        expected = {21475, 21476};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(5251)));

        expected = {23657, 23659, 23660, 23661, 23662, 23669, 23673, 23675, 23679, 23686, 30814, 53050};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(5810)));

        expected = {25354, 26368, 26371, 26372, 26373};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(6335)));

        expected = {25452, 25455};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(6118)));

        expected = {46278, 73371, 73397};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(17005)));

        expected = {48816, 49449, 50368, 51650, 58156, 58175, 58193, 58220};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(9763)));

        expected = {53976, 53981};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(11254)));

        expected = {56567, 56667};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(12187)));

        expected = {57751, 57753};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(12410)));

        expected = {59378, 59379};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(12960)));

        expected = {59722, 59868};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(13071)));

        expected = {68220, 68350, 68632, 68774};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(15369)));

        expected = {68360, 68830};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(15513)));

        expected = {69617, 116982, 129107, 129108, 136849};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(21840)));

        expected = {69987, 69988, 69989};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(15907)));

        expected = {70533, 70540, 70564, 70567, 70586, 70598, 70599};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(16047)));

        expected = {71615, 71616, 71617, 71618, 71619, 71620};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(16409)));

        expected = {78599, 78600, 78610, 78611, 78617, 78618, 78619, 78620, 78621, 78623, 78627, 78636};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(17667)));

        expected = {80386, 80387, 80393, 80394};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(17962)));

        expected = {81235, 81236, 81392};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(18311)));

        expected = {88273, 91691, 91702};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(20146)));

        expected = {91287, 91314};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(20088)));

        expected = {133906, 136983, 136985, 136986, 136987, 136988};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfStreet(22438)));

        //Generate random inputs
        std::vector<StreetIdx> street_ids;
        for(size_t i = 0; i < 1600000; i++) {
            street_ids.push_back(rand_street(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(1481);
            std::vector<IntersectionIdx> result;
            for(size_t i = 0; i < 1600000; i++) {
                result = findIntersectionsOfStreet(street_ids[i]);
            }
        }
    } //all_street_intersections_perf

    TEST_FIXTURE(MapFixture, intersection_ids_from_street_ids_perf) {
        //Verify Functionality
        std::vector<IntersectionIdx> expected;

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(4307, 6554))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(6382, 4558))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(7759, 13936))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(8546, 10335))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(9037, 17744))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(10310, 17423))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(11344, 3857))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(13040, 11300))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(13524, 18480))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(15713, 18564))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(16785, 509))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(19179, 17379))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(19739, 409))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(19942, 22145))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(21504, 20443))));

        expected = {16049};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(20298, 3842))));

        expected = {17590};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(4243, 4242))));

        expected = {25452, 25455};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(6118, 980))));

        expected = {30850, 30854, 36602, 36604};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(7409, 0))));

        expected = {33460, 33461, 33464, 33466, 33469};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(7209, 0))));

        expected = {43810};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(8366, 8361))));

        expected = {57751};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(12410, 12406))));

        expected = {64389};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(14345, 14342))));

        expected = {67930};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(15277, 15276))));

        expected = {70599};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(16047, 16077))));

        expected = {71620};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(16409, 4507))));

        expected = {72383};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(16652, 16649))));

        expected = {78729};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(17700, 20337))));

        expected = {81079};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(18188, 18179))));

        expected = {81392};
        ECE297_CHECK_EQUAL(expected, sorted(findIntersectionsOfTwoStreets(std::make_pair(18311, 18217))));

        //Generate random inputs
        std::vector<StreetIdx> street_ids_1;
        std::vector<StreetIdx> street_ids_2;
        for(size_t i = 0; i < 900000; i++) {
            StreetIdx street_id1 = rand_street(rng);
            StreetIdx street_id2 = rand_street(rng);
            street_ids_1.push_back(street_id1);
            street_ids_2.push_back(street_id2);
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(491);
            std::vector<IntersectionIdx> result;
            for(size_t i = 0; i < 900000; i++) {
                result = findIntersectionsOfTwoStreets(std::make_pair(street_ids_1[i], street_ids_2[i]));
            }
        }
    } //intersection_ids_from_street_ids_perf

    TEST_FIXTURE(MapFixture, street_ids_from_partial_street_name_perf) {
        //Verify Functionality
        std::vector<StreetIdx> expected;

        expected = {1, 2, 3, 4, 9, 31, 42, 61, 62, 119, 123, 124, 125, 128, 129, 130, 132, 133, 142, 143, 144, 145, 146, 147, 148, 149, 313, 314, 390, 507, 567, 570, 621, 622, 755, 900, 901, 902, 967, 1389, 1395, 1694, 1721, 1850, 1851, 1852, 1853, 1854, 2081, 2082, 2083, 2155, 2169, 2276, 2444, 2576, 2638, 2639, 2725, 2759, 3069, 3084, 3388, 3505, 3796, 3934, 4001, 5028, 5052, 5219, 5572, 5807, 5955, 5992, 6127, 6229, 6230, 6573, 6698, 6773, 6774, 6775, 7166, 7932, 8592, 8605, 8715, 8924, 9915, 10485, 10678, 11046, 11053, 11445, 11501, 11904, 11998, 12024, 12295, 12315, 12505, 12777, 12819, 13309, 13673, 13793, 13909, 14434, 15243, 15608, 15939, 16602, 16614, 16869, 17481, 17523, 18408, 18879, 18961, 19576, 20167, 21340, 21600, 21656, 21738, 21774, 21801, 21802, 22746};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("High")));

        expected = {30, 59, 99, 246, 257, 263, 356, 427, 434, 454, 781, 846, 863, 878, 910, 1295, 1297, 1299, 1301, 1603, 1605, 1651, 1682, 1758, 1828, 1830, 1868, 1982, 2089, 2333, 2430, 2513, 2522, 2546, 2556, 2716, 2720, 2755, 2816, 2851, 2881, 3075, 3152, 3218, 3275, 3382, 3410, 3413, 3460, 3485, 3542, 3547, 3595, 3665, 3731, 3734, 3840, 4011, 4022, 4073, 4127, 4268, 4269, 4357, 4363, 4377, 4457, 4509, 4589, 4724, 4878, 5015, 5084, 5274, 5278, 5504, 5517, 5645, 5716, 5725, 6017, 6018, 6030, 6252, 6270, 6282, 6297, 6369, 6397, 6552, 6613, 6646, 6809, 6818, 6857, 6897, 6996, 7006, 7110, 7277, 7286, 7344, 7390, 7391, 7411, 7533, 7561, 7723, 7755, 7756, 7988, 8021, 8025, 8151, 8180, 8488, 8538, 8588, 8706, 8819, 8866, 9096, 9136, 9285, 9363, 9399, 9429, 9434, 9457, 9540, 9647, 9660, 9788, 9820, 9964, 10044, 10067, 10077, 10118, 10120, 10165, 10201, 10225, 10278, 10317, 10319, 10513, 10640, 10654, 10694, 10731, 10746, 10795, 10847, 10870, 11022, 11039, 11089, 11128, 11160, 11231, 11333, 11447, 11468, 11471, 11491, 11561, 11625, 11706, 11799, 11882, 11974, 12018, 12033, 12062, 12080, 12113, 12184, 12379, 12410, 12472, 12525, 12535, 12656, 12884, 12910, 12952, 13009, 13037, 13333, 13402, 13435, 13451, 13545, 13561, 13580, 13720, 13984, 13993, 14032, 14089, 14404, 14445, 14448, 14485, 14568, 14570, 14641, 14775, 14803, 14910, 14922, 14946, 15223, 15293, 15387, 15456, 15637, 15704, 15760, 15806, 15836, 15844, 16059, 16195, 16318, 16412, 16425, 16451, 16491, 16520, 16624, 16637, 16642, 16663, 16757, 16857, 17030, 17033, 17043, 17181, 17271, 17280, 17448, 17539, 17560, 17580, 17598, 17671, 17687, 17733, 17768, 17770, 17843, 17848, 18100, 18387, 18506, 18525, 18534, 18550, 18562, 18574, 18613, 18616, 18634, 18640, 18675, 18710, 18892, 19060, 19171, 19300, 19523, 19587, 19745, 20014, 20047, 20143, 20155, 20225, 20351, 20390, 20443, 20504, 20606, 20740, 20754, 20776, 20883, 20887, 20888, 20889, 20907, 21176, 21210, 21386, 21424, 21437, 21478, 21579, 21633, 21675, 21685, 21851, 21852, 21853, 21908, 21996, 22012, 22066, 22368, 22369, 22707, 22729, 22739, 22753, 22759, 22763};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Pa")));

        expected = {94, 100, 104, 105, 1273, 5454, 5463, 6642, 6643, 7388, 9039, 16653, 20161, 20446, 21182, 22713, 22715};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Lake Shor")));

        expected = {354, 379, 484, 722, 744, 1070, 1072, 1203, 1211, 1361, 1545, 1585, 1592, 1814, 1837, 1912, 2062, 2619, 2870, 2981, 3096, 3142, 3297, 3524, 3525, 3526, 3792, 3861, 3895, 4331, 4367, 4596, 4629, 4765, 4910, 4925, 5071, 5281, 5411, 5430, 5438, 5730, 5768, 5779, 5857, 5916, 6075, 6172, 6463, 6496, 6564, 6621, 6705, 7130, 7177, 7199, 7574, 7620, 8515, 8622, 8664, 8750, 8813, 8876, 9030, 9070, 9236, 9345, 9383, 9495, 9521, 9596, 9615, 9724, 9763, 9772, 9796, 9802, 9895, 10075, 10203, 10224, 10419, 10549, 10627, 10913, 10939, 11150, 11273, 11348, 11350, 11492, 11499, 11647, 11675, 11785, 12041, 12405, 12482, 12485, 12523, 12577, 12683, 12754, 12940, 13114, 13180, 13431, 13606, 13635, 13749, 13835, 13971, 14100, 14265, 14388, 14778, 14863, 14871, 15112, 15333, 15337, 15368, 15442, 15545, 15634, 15748, 15827, 15848, 15969, 16022, 16106, 16133, 16193, 16232, 16264, 16495, 16714, 16739, 16801, 16974, 17090, 17207, 17424, 17439, 17490, 17493, 17589, 17606, 17654, 17981, 18024, 18219, 18383, 18390, 18476, 18641, 19095, 19234, 19407, 19528, 20042, 20049, 20083, 20112, 20190, 20313, 20524, 20584, 20849, 20919, 21119, 21388, 21612, 21668, 21785, 21823, 21844, 21850, 21923, 22328, 22352, 22638};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Fa")));

        expected = {549, 550, 1535, 1536, 6124};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Linsmore Crescent")));

        expected = {1110, 4069};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Eglinton Square")));

        expected = {1960, 17379};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Saint Clements Avenue")));

        expected = {2021, 4558, 7205, 10744, 11131, 11612, 15774, 17548, 20118};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Cove")));

        expected = {2549, 3680, 17648};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Sutherla")));

        expected = {2917, 5092, 5392, 5861, 6488, 6641, 7237, 7424, 7508, 10285, 10406, 11867, 11975, 13574, 13766, 15277, 15538, 15846, 15851, 16322, 16541, 16999, 18823, 18832, 18839, 18950, 20082, 20650, 21068};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Apple ")));

        expected = {4964};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Lane South Dundas East Beaconsfi")));

        expected = {5228, 5518, 5520, 8065, 9203, 11614, 11949, 12016, 12902, 13626, 16344, 16347, 18418, 20021, 20198};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Tree")));

        expected = {5251};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Director Court")));

        expected = {5374, 6923, 21263, 22623};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Woodbine Track ")));

        expected = {6335};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Grand Highland Way")));

        expected = {6397};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Park Aven")));

        expected = {7255};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Hiawatha Parkway")));

        expected = {8369, 22145};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Crestview Road")));

        expected = {8794, 16652};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Cristi")));

        expected = {8800};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Via Nova Drive")));

        expected = {9037};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Rideau Gate")));

        expected = {11067, 17005};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Basildon Crescent")));

        expected = {12187};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Caddy Drive")));

        expected = {12390};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Graybark Crescent")));

        expected = {13040};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Kendlet")));

        expected = {13071};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Davistow Cre")));

        expected = {15369};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Shelford Terra")));

        expected = {16047};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Brookhurst Road")));

        expected = {18188};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Basilic")));

        expected = {19739};
        ECE297_CHECK_EQUAL(expected, sorted(findStreetIdsFromPartialStreetName("Callaway Lane")));

        //Generate random inputs
        std::vector<std::string> street_names_prefixes;
        for(size_t i = 0; i < 1000000; i++) {
            std::string rand_street_name = getStreetName(rand_street(rng));
            unsigned rand_prefix_end_pos = (1.0*rng()/rng.max())*rand_street_name.length();
            street_names_prefixes.push_back(rand_street_name.substr(0, rand_prefix_end_pos));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(19475);
            std::vector<StreetIdx> result;
            for(size_t i = 0; i < 1000000; i++) {
                result = findStreetIdsFromPartialStreetName(street_names_prefixes[i]);
            }
        }
    } //street_ids_from_partial_street_name_perf

} //street_queries_perf_public_toronto_canada

