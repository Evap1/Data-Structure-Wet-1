#include <iostream>
#include "StreamingDBa1.h"

void get_all_movies_aux(streaming_database *obj, int genre);
void my_query_get_all_movies_aux(streaming_database *obj, Genre genre);

void get_all_movies_aux(streaming_database *obj, int genre) {
    my_query_get_all_movies_aux(obj, static_cast<Genre>(genre));
}

void add_movie_aux(streaming_database *obj, int movieId, int genre, int views, bool isVip) {
    obj->add_movie(movieId, static_cast<Genre>(genre), views, isVip);
}

void get_num_views_aux(streaming_database *obj, int movieId, int genre) {
    output_t<int> count = obj->get_num_views(movieId, static_cast<Genre>(genre));
    std::cout << count.ans() << std::endl;
}

void get_all_movies_count_aux(streaming_database *obj, int genre) {
    obj->get_all_movies_count(static_cast<Genre>(genre));
}


void my_query_get_all_movies_aux(streaming_database *obj, Genre genre)
{
    output_t<int> count = obj->get_all_movies_count(genre);
    int to_alloc = count.ans();
    if (to_alloc == 0)
    {
        //if there are no movies we will allocate 1 so that we will get failure instead of invalid input
        ++to_alloc;
    }
    // Allocate if okay
    int *out_mem = nullptr;
    if (count.status() == StatusType::SUCCESS)
    {
        out_mem = new int[to_alloc];
        for (int i = 0; i < to_alloc; ++i) out_mem[i] = -1;
    }
    // Call function
    StatusType status = obj->get_all_movies(genre, out_mem);
//    print(cmd, status);
    if (status == StatusType::SUCCESS) {
        for (int i = 0; i < to_alloc; ++i)
        {
            std::cout << out_mem[i] << std::endl;
        }
    }
    delete[] out_mem;
}


void test() {
    streaming_database *obj = new streaming_database();


    get_num_views_aux(obj, 1044, 3);
    obj->group_watch(8523, 8077);
    obj->add_group(7599);
    obj->group_watch(7599, 7040);
    add_movie_aux(obj, 9467, 0, 91, false);
    obj->user_watch(7802, 9467);
    obj->remove_movie(9467);
    obj->add_group(9984);
    obj->user_watch(6406, 3881);
    get_all_movies_count_aux(obj, 4);
    obj->remove_user(3186);
    obj->add_user(9405, true);
    obj->user_watch(9405, 8436);
    obj->remove_user(9405);
    add_movie_aux(obj, 7455, 2, -2, false);
    get_num_views_aux(obj, 9405, 1);
    obj->add_user_to_group(9405, 7599);
    add_movie_aux(obj, 7950, 4, 67, true);
    add_movie_aux(obj, 1530, 0, 3, false);
    obj->group_watch(7599, 1530);
    obj->add_group(8420);
    obj->remove_group(8420);
    get_all_movies_count_aux(obj, 1);
    obj->group_watch(9984, 1530);
    get_num_views_aux(obj, 9405, 4);
    obj->get_group_recommendation(9984);
    add_movie_aux(obj, 8708, 1, 13, false);
    obj->add_user(4417, true);
    get_all_movies_aux(obj, 3);
    obj->remove_movie(7455);
    obj->add_group(369);
    obj->remove_movie(1530);
    add_movie_aux(obj, 1348, 4, -3, false);
    get_all_movies_count_aux(obj, 1);
    obj->group_watch(369, 7455);
    obj->remove_user(9405);
    obj->remove_group(7599);
    obj->user_watch(9405, 1348);
    obj->remove_user(4417);
    get_all_movies_aux(obj, 0);
    obj->add_user_to_group(9405, 9984);
    get_all_movies_aux(obj, 0);
    obj->add_user_to_group(9405, 9984);
    add_movie_aux(obj, 6337, 0, 34, true);
    obj->add_group(8614);
    obj->add_user(876, true);
    obj->rate_movie(876, 7455, 99);
    obj->group_watch(369, 1348);
    obj->add_user(1359, true);
    obj->remove_user(876);
    obj->group_watch(369, 8708);
    obj->remove_group(8614);
    obj->add_user(6203, false);
    obj->remove_user(9405);
    get_all_movies_aux(obj, 2);
    obj->add_group(5144);
    obj->add_group(1213);
    get_num_views_aux(obj, 1359, 2);
    obj->user_watch(1359, 6337);
    obj->add_group(1551);
    obj->add_user(3366, true);
    obj->get_group_recommendation(1213);
    obj->rate_movie(1359, 7455, 89);
    get_num_views_aux(obj, 3366, 4);
    obj->group_watch(1551, 7950);
    get_all_movies_count_aux(obj, 1);
    obj->add_user(2653, false);
    obj->rate_movie(9405, 8708, 114);
    obj->add_group(2412);
    obj->add_user_to_group(2653, 9984);
    obj->user_watch(9405, 7950);
    obj->add_user(1083, false);
    obj->add_user(4491, false);
    obj->group_watch(2412, 1348);
    obj->user_watch(1359, 7950);
    obj->remove_movie(7950);
    obj->remove_user(1083);
    obj->add_user_to_group(1083, 9984);
    obj->rate_movie(1083, 7455, 146);
    get_all_movies_count_aux(obj, 4);
    obj->add_user(4493, true);
    obj->add_group(4517);
    add_movie_aux(obj, 5555, 1, 6, false);
    obj->remove_group(5144);
    obj->remove_group(9984);
    obj->user_watch(6203, 1348);
    obj->add_user(3442, false);
    obj->add_user_to_group(1083, 369);
    obj->add_group(8474);
    obj->remove_user(2653);
    get_all_movies_count_aux(obj, 1);
    obj->group_watch(1551, 1348);
    obj->user_watch(4493, 8708);
    obj->rate_movie(3366, 5555, 84);
    obj->add_user(5090, true);
    obj->group_watch(4517, 6337);
    add_movie_aux(obj, 5370, 3, 40, false);
    obj->add_user(3325, true);
    obj->add_user(2224, true);
    obj->remove_user(1083);
    obj->add_group(425);
    obj->user_watch(1083, 8708);
    obj->add_group(1213);
    obj->remove_movie(5555);
    obj->add_user_to_group(3325, 425);
    obj->add_group(1322);
    obj->add_group(4706);
    obj->group_watch(1322, 8708);
    obj->user_watch(4493, 6337);
    obj->remove_group(1213);
    obj->remove_user(2224);
    obj->remove_user(5090);
    get_all_movies_count_aux(obj, 4);
    obj->user_watch(3366, 5370);
    add_movie_aux(obj, 9366, 3, 95, false);
    get_all_movies_aux(obj, 4);
    obj->add_user(9839, true);
    obj->group_watch(2412, 5370);
    obj->remove_movie(6337);
    get_num_views_aux(obj, 9405, 2);
    obj->add_group(7025);
    obj->remove_group(369);
    obj->rate_movie(9839, 8708, 70);
    obj->remove_group(8474);
    obj->add_user(6235, true);
    get_all_movies_aux(obj, 3);
    obj->add_group(3238);
    get_all_movies_count_aux(obj, 1);
    obj->add_user_to_group(3442, 1322);
    obj->user_watch(1083, 1348);
    obj->add_user(9324, true);
    get_all_movies_aux(obj, 1);
    add_movie_aux(obj, 6746, 3, 41, false);
    obj->remove_group(4517);
    add_movie_aux(obj, 3682, 0, 92, true);
    add_movie_aux(obj, 1528, 2, 12, true);
    obj->group_watch(4706, 1528);
    obj->rate_movie(3366, 6746, 25);
    obj->user_watch(9839, 3682);
    obj->get_group_recommendation(1551);
    get_num_views_aux(obj, 9839, 3);
    obj->add_user(8272, false);
    get_all_movies_count_aux(obj, 4);
    obj->add_user(7437, true);
    get_all_movies_aux(obj, 2);
    obj->remove_user(1083);
    obj->add_user(1092, false);
    add_movie_aux(obj, 4518, 2, 38, false);
    obj->get_group_recommendation(425);
    obj->add_group(8772);


//    get_all_movies_count_aux(obj, 2);
//    obj->rate_movie(1634, 7872, 142);
//    obj->add_user_to_group(9305, 453);
//    obj->group_watch(7081, 5015);
//    obj->add_user(9617, false);
//    get_all_movies_count_aux(obj, 4);
//    add_movie_aux(obj, 8722, 2, 26, true);
//    obj->add_user_to_group(9617, 7367);
//    get_num_views_aux(obj, 9617, 3);
//    obj->add_group(8812);
//    obj->get_group_recommendation(8812);
//    obj->get_group_recommendation(8812);
//    obj->add_group(2461);
//    obj->rate_movie(9617, 8722, 29);
//    obj->get_group_recommendation(8812);
//    obj->add_group(8812);
//    obj->remove_group(8812);
//    get_all_movies_aux(obj, 1);
//    obj->add_user_to_group(9617, 2461);
//    get_num_views_aux(obj, 9617, 4);
//    add_movie_aux(obj, 9301, 2, 37, true);
//    add_movie_aux(obj, 5077, 3, 27, true);
//    add_movie_aux(obj, 8180, 3, 12, true);
//    obj->group_watch(2461, 8180);
//    get_all_movies_aux(obj, 3);
//    obj->group_watch(2461, 8722);
//    obj->remove_user(9617);
//    get_num_views_aux(obj, 6151, 3);
//    obj->add_user(3643, true);
//    obj->add_user(6854, true);
//    get_all_movies_aux(obj, 1);
//    obj->user_watch(3643, 8180);
//    obj->user_watch(3643, 8722);
//    get_all_movies_aux(obj, 1);
//    obj->add_group(2471);
//    add_movie_aux(obj, 3826, 2, 41, false);
//    add_movie_aux(obj, 1407, 2, 41, true);
//    obj->remove_movie(5077);
//    obj->add_user(9314, false);
//    add_movie_aux(obj, 6434, 4, 37, false);
//    get_all_movies_aux(obj, 1);
//    obj->remove_group(2461);
//    obj->user_watch(9314, 9301);
//    obj->add_user(7091, false);
//    obj->rate_movie(6854, 6434, 78);
//    add_movie_aux(obj, 8523, 1, 78, false);
//    obj->add_group(4359);
//    add_movie_aux(obj, 716, 0, 33, false);
//    add_movie_aux(obj, 7289, 2, 91, true);
//    add_movie_aux(obj, 1575, 4, 61, false);
//    get_all_movies_count_aux(obj, 3);
//    get_all_movies_count_aux(obj, 1);
//    get_num_views_aux(obj, 7091, 1);
//    obj->remove_movie(8722);
//    obj->add_user(7796, true);
//    obj->add_user(6289, false);
//    get_num_views_aux(obj, 7091, 4);
//    obj->add_user(3040, false);
//    obj->remove_user(6854);
//    obj->rate_movie(3643, 716, 107);
//    obj->add_user_to_group(6289, 2471);
//    obj->remove_group(2471);
//    obj->remove_group(4359);
//    obj->add_user(4506, false);
//    get_all_movies_aux(obj, 4);
//    get_all_movies_aux(obj, 2);
//    add_movie_aux(obj, 2829, 4, 20, false);
//    obj->get_group_recommendation(4359);
//    obj->remove_user(6289);
//    obj->add_group(9206);
//    obj->add_user(6947, true);
//    get_all_movies_aux(obj, 0);
//    get_all_movies_aux(obj, 2);
//    get_all_movies_aux(obj, 4);
//    obj->add_group(1712);
//    obj->add_user(9874, false);
//    get_num_views_aux(obj, 9314, 1);
//    get_all_movies_count_aux(obj, 4);
//    get_all_movies_aux(obj, 1);
//    get_all_movies_count_aux(obj, 2);
//    obj->rate_movie(9314, 2829, 57);
//    obj->add_user_to_group(3040, 4359);
//    obj->user_watch(7091, 1407);
//    obj->user_watch(9314, 1407);
//    obj->remove_group(9206);
//    get_all_movies_count_aux(obj, 0);
//    obj->remove_movie(716);
//    obj->add_user(9388, false);
//    obj->remove_group(4359);
//    obj->remove_user(4506);


//    obj->add_group(6567);
//    get_all_movies_aux(obj, 1);
//    get_all_movies_aux(obj, 3);
//    get_all_movies_aux(obj, 0);
//    get_num_views_aux(obj, 9854, 2);
//    get_num_views_aux(obj, 1472, 2);
//    obj->add_user_to_group(3347, 8466);
//    obj->add_group(7703);
//    obj->add_user(9744, false);
//    obj->add_user_to_group(9744, 6567);
//    obj->add_group(6727);
//    get_all_movies_aux(obj, 3);
//    obj->rate_movie(9744, 5173, 2);
//    obj->group_watch(7703, 6945);
//    obj->group_watch(6727, 264);
//    obj->remove_user(9744);
//    get_all_movies_count_aux(obj, 2);
//    obj->rate_movie(6756, 4670, 116);
//    get_all_movies_count_aux(obj, 4);
//    obj->add_user(8287, false);
//    get_num_views_aux(obj, 8287, 3);
//    obj->group_watch(7703, 7932);
//    obj->add_group(9143);
//    obj->remove_movie(1422);
//    get_num_views_aux(obj, 8287, 0);
//    add_movie_aux(obj, 7427, 3, 90, true);
//    obj->add_user_to_group(8287, 7703);
//    obj->add_user(8857, true);
//    get_all_movies_count_aux(obj, 1);
//    obj->rate_movie(8287, 7427, 136);
//    add_movie_aux(obj, 1138, 2, 4, false);
//    obj->remove_group(9143);
//    obj->remove_movie(7427);
//    get_num_views_aux(obj, 8287, 3);
//    add_movie_aux(obj, 5174, 1, 12, false);
//    obj->remove_movie(5174);
//    obj->add_user_to_group(8287, 6727);
//    get_all_movies_aux(obj, 4);
//    add_movie_aux(obj, 1085, 0, 80, false);
//    get_all_movies_aux(obj, 2);
//    obj->add_user_to_group(8857, 7703);
//    obj->add_group(4884);
//    get_all_movies_aux(obj, 0);
//    obj->group_watch(6727, 1085);
//    get_all_movies_aux(obj, 3);
//    get_num_views_aux(obj, 2050, 3);
//    obj->remove_movie(1138);
//    obj->user_watch(8287, 7928);
//    obj->add_group(8237);
//    get_all_movies_aux(obj, 2);
//    obj->remove_user(8287);
//    add_movie_aux(obj, 147, 2, 58, false);
//    obj->add_group(3761);
//    obj->add_user(635, false);
//    get_all_movies_count_aux(obj, 3);
//    get_all_movies_aux(obj, 2);
//    get_all_movies_aux(obj, 0);
//    obj->add_group(2125);
//    add_movie_aux(obj, 9750, 2, 6, true);
//    obj->add_user_to_group(8857, 6727);


    delete obj;

}


int main() {

    test();
    return 0;
}
