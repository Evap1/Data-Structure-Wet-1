// 
// 234218 Data Structures 1.
// Semester: 2023B (spring).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef STREAMINGDBA1_H_
#define STREAMINGDBA1_H_

#include "wet1util.h"
#include "AVL.h"
#include "Group.h"
#include "User.h"
#include "Movie.h"




class streaming_database {
private:
	//
	// Here you may add anything you want
	//
	
    // None holds the whole tree sorted by : TBD
    TreeNode<Movie>* moviesByRateing[(int)(Genre::NONE)+1];
	TreeNode<Movie>* moviesByID[(int)(Genre::NONE)+1];
    TreeNode<User> users;
    TreeNode<Group> groups;

// Equalitu
enum struct FunctionType {
    INSERT           = 0,
    REMOVE            = 1,
    RATE_MOVIE           = 2,
	UPDATE_VIEWS		= 3
};




int get_all_movies_inside(const Node<Movie>* moviesRoot, int *const output);
StatusType insert_to_streamming_trees(const Movie &movie);
StatusType do_to_all_4_trees(Node<Movie>* node, int rate , FunctionType function);





	
public:
	// <DO-NOT-MODIFY> {
	
	streaming_database();

	virtual ~streaming_database();
	
	StatusType add_movie(int movieId, Genre genre, int views, bool vipOnly);
	
	StatusType remove_movie(int movieId);
	
	StatusType add_user(int userId, bool isVip);
	
	StatusType remove_user(int userId);
	
	StatusType add_group(int groupId);
	
	StatusType remove_group(int groupId);

	StatusType add_user_to_group(int userId, int groupId);
	
	StatusType user_watch(int userId, int movieId);

    StatusType group_watch(int groupId,int movieId);

    output_t<int> get_all_movies_count(Genre genre);

    StatusType get_all_movies(Genre genre, int *const output);
	
	output_t<int> get_num_views(int userId, Genre genre);

    StatusType rate_movie(int userId, int movieId, int rating);
	
	output_t<int> get_group_recommendation(int groupId);
	
	// } </DO-NOT-MODIFY>
};

#endif // STREAMINGDBA1_H_
