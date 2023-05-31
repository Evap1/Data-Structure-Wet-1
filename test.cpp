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

    obj->add_group(6567);
    get_all_movies_aux(obj, 1);
    get_all_movies_aux(obj, 3);
    get_all_movies_aux(obj, 0);
    get_num_views_aux(obj, 9854, 2);
    get_num_views_aux(obj, 1472, 2);
    obj->add_user_to_group(3347, 8466);
    obj->add_group(7703);
    obj->add_user(9744, false);
    obj->add_user_to_group(9744, 6567);
    obj->add_group(6727);
    get_all_movies_aux(obj, 3);
    obj->rate_movie(9744, 5173, 2);
    obj->group_watch(7703, 6945);
    obj->group_watch(6727, 264);
    obj->remove_user(9744);
    get_all_movies_count_aux(obj, 2);
    obj->rate_movie(6756, 4670, 116);
    get_all_movies_count_aux(obj, 4);
    obj->add_user(8287, false);
    get_num_views_aux(obj, 8287, 3);
    obj->group_watch(7703, 7932);
    obj->add_group(9143);
    obj->remove_movie(1422);
    get_num_views_aux(obj, 8287, 0);
    add_movie_aux(obj, 7427, 3, 90, true);
    obj->add_user_to_group(8287, 7703);
    obj->add_user(8857, true);
    get_all_movies_count_aux(obj, 1);
    obj->rate_movie(8287, 7427, 136);
    add_movie_aux(obj, 1138, 2, 4, false);
    obj->remove_group(9143);
    obj->remove_movie(7427);
    get_num_views_aux(obj, 8287, 3);
    add_movie_aux(obj, 5174, 1, 12, false);
    obj->remove_movie(5174);
    obj->add_user_to_group(8287, 6727);
    get_all_movies_aux(obj, 4);
    add_movie_aux(obj, 1085, 0, 80, false);
    get_all_movies_aux(obj, 2);
    obj->add_user_to_group(8857, 7703);
    obj->add_group(4884);
    get_all_movies_aux(obj, 0);
    obj->group_watch(6727, 1085);
    get_all_movies_aux(obj, 3);
    get_num_views_aux(obj, 2050, 3);
    obj->remove_movie(1138);
    obj->user_watch(8287, 7928);
    obj->add_group(8237);
    get_all_movies_aux(obj, 2);
    obj->remove_user(8287);
    add_movie_aux(obj, 147, 2, 58, false);
    obj->add_group(3761);
    obj->add_user(635, false);
    get_all_movies_count_aux(obj, 3);
    get_all_movies_aux(obj, 2);
    get_all_movies_aux(obj, 0);
    obj->add_group(2125);
    add_movie_aux(obj, 9750, 2, 6, true);
    obj->add_user_to_group(8857, 6727);



    delete obj;

}


int main() {

    test();
    return 0;
}
