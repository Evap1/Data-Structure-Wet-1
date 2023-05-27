#include "StreamingDBa1.h"
#include "AVL.h"
StatusType correct_status(StatusType status1,StatusType status2);

streaming_database::streaming_database()
{
	// TODO: Your code goes here
}

streaming_database::~streaming_database()
{
	// TODO: Your code goes here
}

StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
	if(movieId <= 0 || genre == Genre::NONE ||  views < 0)
		return StatusType::INVALID_INPUT;

	Movie* m = new Movie(movieId,genre,views, vipOnly);
	if(moviesByGenre[(int)Genre::NONE]->find(*m) == NULL)
	{
		StatusType status1 = moviesByGenre[(int)genre]->insert(*m);
		StatusType status2 = moviesByGenre[(int)Genre::NONE]->insert(*m);
		return correct_status(status1, status2);

	}

	return StatusType::FAILURE;


	// TODO: Your code goes here
	//return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
	if(movieId <=0)
		return StatusType::INVALID_INPUT;
	Movie* m = new Movie(movieId,Genre::NONE,0, false);
	Node<Movie>* node =  moviesByGenre[(int)Genre::NONE]->findBy(*m, IdSearch);
	if(node != NULL)
	{
		StatusType status1 = moviesByGenre[(int)node->get_key().getGenre()]->remove(*m);
		StatusType status2 = moviesByGenre[(int)Genre::NONE]->remove(*m);
		return correct_status(status1, status2);
	}

	return moviesByGenre[(int)Genre::NONE]->remove(*m);

	/// TODO: need to create a function that can find a movie by its id only!
	// TODO: Your code goes here
}

/// @brief add new user to users tree.
/// @param userId
/// @param isVip
/// @return Status of the operation
StatusType streaming_database::add_user(int userId, bool isVip)
{
	if (userId <= 0) return StatusType::INVALID_INPUT;
    return users.insert(User(userId,isVip));
}

/// @brief remove existing user from users tree.
/// @param userId
/// @return Status of the operation
StatusType streaming_database::remove_user(int userId)
{
    if (userId <= 0) return StatusType::INVALID_INPUT;
	return users.remove(User(userId));
}

/// @brief add an empty new group to groups tree.
/// @param groupId
/// @return Status of the operation
StatusType streaming_database::add_group(int groupId)
{
    if (groupId <= 0) return StatusType::INVALID_INPUT;
	return groups.insert(Group(groupId));
}

/// @brief
/// @param groupId
/// @return
//TODO: need to fill array with users and set their group to NONE
StatusType streaming_database::remove_group(int groupId)
{
    Node<Group>* toDelete = groups.find(Group(groupId));

	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
	// TODO: Your code goes here
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
	// TODO: Your code goes here
    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{

	if(moviesByGenre[(int)genre] != NULL && moviesByGenre[(int)genre]->get_root() != NULL)
    {
		output_t<int>* temp = new output_t<int>(moviesByGenre[(int)genre]->get_counter());
			return *temp;

    }
	output_t<int>* temp = new output_t<int>(0);
	return *temp;

    // // TODO: Your code goes here
    // output[0] = 4001;
    // output[1] = 4002;
    return StatusType::SUCCESS;
}


StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
	if(output == NULL)
		return StatusType::INVALID_INPUT;
	
	if((genre == Genre::NONE && get_all_movies_count(genre).ans() <=0) 
		||(genre != Genre::NONE && get_all_movies_count(genre).ans() <=0 ))
			return StatusType::FAILURE;

	if(moviesByGenre[(int)genre]->get_root() != NULL)
	{
		get_all_movies_inside(moviesByGenre[(int)genre]->get_root(), output);
		return StatusType::SUCCESS;
	}

    // // TODO: Your code goes here
    // output[0] = 4001;
    // output[1] = 4002;
}

/// Inside private function!
/// @brief An inside functuion that recursively puts the movies ID's in output array by inorder sort
/// @param moviesTree 
/// @param output 
/// @return The return is for the rcursion for the indexing in the array
int streaming_database::get_all_movies_inside(const Node<Movie>* moviesRoot, int *const output)
{

	/// TODO: create a get left and get right function to the tree
	if (moviesRoot == NULL){
		return 0;
	}

	int i = get_all_movies_inside(moviesRoot->get_left() , output);
	output[i] = moviesRoot->get_key().getMovieId();
	i++;
	i += get_all_movies_inside(moviesRoot->get_right() , output);
	return i;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
	// TODO: Your code goes here
	return 2008;
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
	if(userId<=0 || movieId <=0 || rating < 0 || rating > 100)
		return StatusType::INVALID_INPUT;
	
	if((users != NULL) && (!users.findBy((User(userId, false), idSearch))))
		return StatusType::FAILURE;

	if(moviesByGenre[(int)Genre::NONE] != NULL && moviesByGenre[(int)Genre::NONE]->get_root() != NULL)\
	{
		Movie* movie = *(moviesByGenre[(int)Genre::NONE]->findBy(Movie(movieId,Genre::NONE, 0, false), idSearch));
		if(movie != NULL)
		{
			movie->add_rating(rating);
			return StatusType::SUCCESS;
		}
	}

	return StatusType::FAILURE;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}



/// @brief Checks what a function needs to return if there are more than one status type
/// @param status1 
/// @param status2 
/// @return 
StatusType correct_status(StatusType status1,StatusType status2)
{
	if(status1 == StatusType::SUCCESS && status2 == StatusType::SUCCESS)
		return StatusType::SUCCESS;

	if(status1 == StatusType::ALLOCATION_ERROR || status2 == StatusType::ALLOCATION_ERROR)
		return StatusType::ALLOCATION_ERROR;

	if(status1 == StatusType::INVALID_INPUT || status2 == StatusType::INVALID_INPUT)
		return StatusType::INVALID_INPUT;
	
	if(status1 == StatusType::FAILURE || status2 == StatusType::FAILURE)
		return StatusType::FAILURE;

	return StatusType::FAILURE;

}
