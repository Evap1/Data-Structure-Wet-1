#include "StreamingDBa1.h"
#include "AVL.h"
#include <new>

StatusType correct_status(StatusType status1,StatusType status2);

//TODO: may cause problems with new and mempry dealing. users.
/// @brief Constructor for an empty streaming database.
// no need to free elements alloced before, not dynamiclly.
streaming_database::streaming_database() : users(TreeNode<User>()), groups(TreeNode<Group>())
{
    for (int i= 0; i <= (int)Genre::NONE; i++){
        moviesByRateing[i] = new TreeNode<Movie>();
        moviesByID[i] = new TreeNode<Movie>();
        bestMovie[i] = nullptr;
    }
}

/// dont forget to catch status allocation error if failed
/// @brief Release all memory. O(n+k+m)
streaming_database::~streaming_database()
{
    for (int i= 0; i <= (int)Genre::NONE; i++){
		delete moviesByRateing[i];
		delete moviesByID[i];
		if (bestMovie[i] != nullptr){
			delete bestMovie[i];
		}
    }
}

/// @brief add movie by id to all 4 trees. O(logk)
/// @param movieId
/// @param genre
/// @param views
/// @param vipOnly
/// @return Status of the operation
StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
	IdSearch idSearch;

	if(movieId <= 0 || genre == Genre::NONE ||  views < 0)
		return StatusType::INVALID_INPUT;

	Node<Movie>* node = new Node<Movie>(Movie(movieId,genre,views, vipOnly));
	if(moviesByID[(int)Genre::NONE]->findBy(node, idSearch) == NULL)
	{
		return do_to_all_4_movies_trees(node,-1,FunctionType::INSERT);
	}
	return StatusType::FAILURE;
}


/// @brief remove a movie from all 4 trees. O(logk)
/// @param movieId
/// @return Status of the operation
StatusType streaming_database::remove_movie(int movieId)
{
	try{
        if(movieId <=0)
            return StatusType::INVALID_INPUT;

        IdSearch idSearch;

        Node<Movie>* temp = new Node<Movie>(Movie(movieId,Genre::NONE,0, false));
        Node<Movie>* node = moviesByID[(int)Genre::NONE]->findBy(temp, idSearch);
        if(node != NULL)
        {
            delete temp;
            return do_to_all_4_movies_trees(node, -1, FunctionType::REMOVE);
        }
        delete temp;
        return StatusType::FAILURE;
	}
	catch(...)
	{
		return StatusType::ALLOCATION_ERROR;
	}
}

/// @brief add new user to users tree. O(logn)
/// @param userId
/// @param isVip
/// @return Status of the operation
StatusType streaming_database::add_user(int userId, bool isVip)
{
	if (userId <= 0) return StatusType::INVALID_INPUT;
    return users.insert(User(userId,isVip));
}

/// @brief remove existing user from users tree. O(logn)
/// @param userId
/// @return Status of the operation
StatusType streaming_database::remove_user(int userId)
{
    if (userId <= 0) return StatusType::INVALID_INPUT;
    Node<User>* toRemove = users.find(User(userId));

    //TODO: if user is part of group, can't delete
    Group* toUpdate = toRemove->get_key_by_ref()->get_group_ptr();
    if ( toRemove->get_key_by_ref()->get_group_id() != User::NONE){
        for (int i= 0; i <= (int)Genre::NONE; i++){
            int userViewsAlone = toRemove->get_key_by_ref()->get_views_per_genre((Genre)i);
            int userViewsWhenJoin = toRemove->get_key_by_ref()->get_views_when_leave((Genre)i);
            int totalViewsGroup = toUpdate->get_movies_as_group((Genre)i);
            int substruct = userViewsAlone + totalViewsGroup - userViewsWhenJoin;
            toUpdate->set_views((Genre)i, -substruct );
        }
    }
	return users.remove(User(userId));
}

/// @brief add an empty new group to groups tree. O(logm)
/// @param groupId
/// @return Status of the operation
StatusType streaming_database::add_group(int groupId)
{
    if (groupId <= 0) return StatusType::INVALID_INPUT;
	return groups.insert(Group(groupId));
}

/// @brief remove node group from the tree group. unsign all memebers
/// related to the group and delete it's tree. O(logm + n(groupUsers))
/// @param groupId
/// @return
//TODO: need to fill array with users and set their group to NONE
StatusType streaming_database::remove_group(int groupId)
{
    if (groupId <= 0) return StatusType::INVALID_INPUT;
    // unassiagn all related users.

//    // find group in the tree. if not found, it's a failure
//    Node<Group>* toDelete = groups.find(Group(groupId));
//    if (toDelete == NULL) return StatusType::FAILURE;

    //toDelete->get_key_by_ref()->empty_group();
    return groups.remove(Group(groupId));
}

/// @brief add user by id , to a user tree inside a group by id. O(logn+logm)
/// @param userId
/// @param groupId
/// @return status if operation
StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    // check input
    if (groupId <= 0 || userId <= 0) return StatusType::INVALID_INPUT;

    // find group in the tree. if not found, it's a failure
    Node<Group>* NodeGroupToUpdate = groups.find(Group(groupId));
    if (NodeGroupToUpdate == NULL) return StatusType::FAILURE;

    // find ptr to user to add. if not found, it's a failure
    User* UserToAdd = users.find(User(userId))->get_key_by_ref();
    if (UserToAdd == NULL) return StatusType::FAILURE;

    // if user has group, it's a failure. otherwise; update the group id of the user
    int UserGroup = UserToAdd->get_group_id();
    if (UserGroup != User::NONE) return StatusType::FAILURE;
    UserToAdd->set_group_id(groupId);

    // add user to the current group tree and fill the movies viewed as group prior to join
    return NodeGroupToUpdate->get_key_to_set().add_user(UserToAdd);
}

//TODO: galor changes
/// @brief Update the views of a user per genre of a movie. updates the views as a group, if related to a group O(logn+logk)
/// @param userId
/// @param movieId
/// @return status of operation
StatusType streaming_database::user_watch(int userId, int movieId) {
    // check input
    if (movieId <= 0 || userId <= 0) return StatusType::INVALID_INPUT;

    // find ptr to user to add. if not found, it's a failure
    User *UserToAdd = users.find(User(userId))->get_key_by_ref();
    if (UserToAdd == NULL) return StatusType::FAILURE;

    // find ptr to user to movie. if not found, it's a failure

    //TODO: add find by id, in NONE tree.
	IdSearch idSearch;
	Node<Movie>* tempMovieNode = new Node<Movie>(Movie(movieId,Genre::NONE,0, false));
	tempMovieNode = moviesByID[(int)Genre::NONE]->findBy(tempMovieNode, idSearch);
    // movie not found
    if (tempMovieNode == NULL) {
        return StatusType::FAILURE;
    }

	Movie *MovieToUpdate = tempMovieNode->get_key_by_ref();

    // check if movie is vip and user allowed to watch
    // case 1: user not allowed:
    if (MovieToUpdate->isVipOnly() && !UserToAdd->is_vip()) {
        delete tempMovieNode;
        return StatusType::FAILURE;
    }

    // case 2: user allowed:
	StatusType tempStatus = do_to_all_4_movies_trees(tempMovieNode, 1, FunctionType::UPDATE_VIEWS);
	if(tempStatus != StatusType::SUCCESS){
        delete tempMovieNode;
        return tempStatus;
    }
    // update views for user
    UserToAdd->add_views_in_genre(MovieToUpdate->getGenre());

    // case 2.1: if related to a group, want to add views to the group as individual.
    if (UserToAdd->get_group_id() != User::NONE){
        Group* GroupToAdd = UserToAdd->get_group_ptr();
        GroupToAdd->set_views_per_movie_user_watch(MovieToUpdate->getGenre());
    }
    delete tempMovieNode;
    return StatusType::SUCCESS;
}

/// @brief Update the views of a group by a genre. O(logm + logk)
/// @param groupId
/// @param movieId
/// @return
StatusType streaming_database::group_watch(int groupId,int movieId)
{
    // check input
    if (movieId <= 0 || groupId <= 0) return StatusType::INVALID_INPUT;

    // find ptr to group to add. if not found, it's a failure
    Group *GroupToAdd = groups.find(Group(groupId))->get_key_by_ref();
    if (GroupToAdd == NULL || GroupToAdd->get_member_count() == 0) return StatusType::FAILURE;

    // find ptr to user to movie. if not found, it's a failure
    //TODO: add find by id, in NONE tree.
    IdSearch idSearch;
	Node<Movie>* tempMovieNode = new Node<Movie>(Movie(movieId,Genre::NONE,0, false));
	tempMovieNode = moviesByID[(int)Genre::NONE]->findBy(tempMovieNode, idSearch);
    if (tempMovieNode == NULL) {
        return StatusType::FAILURE;
    }
	Movie *MovieToUpdate = tempMovieNode->get_key_by_ref();

    // check if movie is vip and user allowed to watch
    // user not allowed
    if (MovieToUpdate->isVipOnly() && !GroupToAdd->is_vip()) {
        delete tempMovieNode;
        return StatusType::FAILURE;
    }
    // TODO: update the counter in all 4 trees
	int viewsCount = GroupToAdd->get_member_count();
	do_to_all_4_movies_trees(tempMovieNode, viewsCount, FunctionType::UPDATE_VIEWS);

    // update views for group
    GroupToAdd->set_views_per_movie(MovieToUpdate->getGenre());

    // TODO: calculate the status
    delete tempMovieNode;
    return StatusType::SUCCESS;
}

/// @brief method to get number of movies in database per genre. O(1)
/// @param genre
/// @return status and number of movies
output_t<int> streaming_database::get_all_movies_count(Genre genre)
{

	if(moviesByRateing[(int)genre] != NULL && moviesByRateing[(int)genre]->get_root() != NULL)
    {
		output_t<int>* temp = new output_t<int>(moviesByRateing[(int)genre]->get_counter());
			return *temp;

    }
	output_t<int>* temp = new output_t<int>(0);
	return *temp;
}

/// @brief fill array with movie ids by desired genre. ordered by rate. O(k (genre))
/// @param genre
/// @param output
/// @return status of operation
StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
	if(output == NULL)
		return StatusType::INVALID_INPUT;
	
	if((genre == Genre::NONE && get_all_movies_count(genre).ans() <=0) 
		||(genre != Genre::NONE && get_all_movies_count(genre).ans() <=0 ))
			return StatusType::FAILURE;

    if(moviesByRateing[(int)genre]->get_root() != NULL)
	{
		get_all_movies_inside(moviesByRateing[(int)genre]->get_root(), output);
		return StatusType::SUCCESS;
	}
    return StatusType::FAILURE;
}

/// Inside private function!
/// @brief An inside functuion that recursively puts the movies ID's in output array by preorder sort
/// @param moviesTree 
/// @param output 
/// @return The return is for the rcursion for the indexing in the array

int streaming_database::get_all_movies_inside(const Node<Movie>* moviesRoot, int *const output)
{

	/// TODO: create a get left and get right function to the tree
	if (moviesRoot == NULL){
		return 0;
	}

	int i = get_all_movies_inside(moviesRoot->get_right() , output);
	output[i] = moviesRoot->get_key().getMovieId();
	i++;
	i += get_all_movies_inside(moviesRoot->get_left() , output);
	return i;
}

/// @brief find the num of views of a user per genre. O(logn)
/// @param userId
/// @param genre
/// @return status and num of views
output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    // check input
    if (userId <= 0) return StatusType::INVALID_INPUT;

    // find ptr to user to add. if not found, it's a failure
    User *UserToAdd = users.find(User(userId))->get_key_by_ref();

    // case 1: user not found:
    if (UserToAdd == NULL) return StatusType::FAILURE;
    // case 2: user found:
    int counter = 0;
    if (genre == Genre::NONE){
        for ( int i = 0; i <= (int)Genre::NONE; i++ ){
            // current user views per genre
            counter += UserToAdd->get_views_per_genre((Genre)(i));
            // add user views as group, if assign to group
            if (UserToAdd->get_group_id()!=User::NONE){
                counter += UserToAdd->get_group_ptr()->get_movies_as_group((Genre)(i));
                counter -= UserToAdd->get_views_when_leave((Genre)(i));
            }
        }
    }
    else {
        counter += UserToAdd->get_views_per_genre(genre);
        // add user views as group, if assign to group
        if (UserToAdd->get_group_id()!=User::NONE) {
            counter += UserToAdd->get_group_ptr()->get_movies_as_group(genre);
            counter -= UserToAdd->get_views_when_leave(genre);
        }
    }
    return {output_t<int>(counter)};

}

/// @brief rate by user, a movie corresponding to an id. setts the average rate. O(logn + logk)
/// @param userId
/// @param movieId
/// @param rating
/// @return status of the operation
StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
	if(userId<=0 || movieId <=0 || rating < 0 || rating > 100)
		return StatusType::INVALID_INPUT;
	
	Node<User>* userNode = users.find(User(userId, false));
	if((users.get_root() != NULL) && (userNode != NULL))
		return StatusType::FAILURE;

    //access the whole tree sorted by rate
    // TODO: why do we need these 2 conditions ? why the 2nd is not enough? (Eva)
	if(moviesByRateing[(int)Genre::NONE] != NULL && moviesByRateing[(int)Genre::NONE]->get_root() != NULL)
	{
		IdSearch idSearch;

        //find movie by rate
		Node<Movie> *temp = new Node<Movie>(Movie(movieId,Genre::NONE, 0, false));
		Node<Movie>* movieNode = moviesByID[(int)Genre::NONE]->findBy(temp, idSearch);
		delete temp;
		if(movieNode->get_key().isVipOnly())
			if(!userNode->get_key().is_vip())
				return StatusType::FAILURE;
		
		if(movieNode != NULL)
		{
			///TODO: need to use do_to_all_4_trees !
			// movieNode->get_key_by_ref()->add_rating(rating); // need changes!
			return do_to_all_4_movies_trees(movieNode, rating,FunctionType::RATE_MOVIE);
		}
	}
	return StatusType::FAILURE;
}


/// @brief give a movieid recommendation for a group, which is the greatest rated movie from the fav genre on a group
/// O(logm)
/// @param groupId
/// @return
output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	if (groupId <=0){
        return {output_t<int>(StatusType::INVALID_INPUT)};
    }
    // find ptr to group. if not found, it's a failure
    Group* GroupToAdd = groups.find(Group(groupId))->get_key_by_ref();

    if (GroupToAdd == NULL || GroupToAdd->get_member_count() == 0)
        return {output_t<int>(StatusType::FAILURE)};

    // find fav genre
    int max = 0;
    Genre favourite = Genre::NONE;
    for (int i = 0; i <= (int)Genre::NONE; i++){
        int current = GroupToAdd->get_views_per_genre((Genre)i);
        if (current > max ){
            max = current;
            favourite = (Genre)i;
        }
        // if equal views, the first in the list is the winner.
        if (current == max ){
            if ((int)favourite > i){
                favourite = (Genre)i;
            }
        }
    }

    // if favourite tree is empty, return failure.
    if (moviesByRateing[(int)favourite] == NULL){
        return {output_t<int>(StatusType::FAILURE)};
    }

    int favId = bestMovie[(int)favourite]->get_key_by_ref()->getMovieId();
    return {output_t<int>(favId)};
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


///TODO: finish this
/// @brief Works by the FunctionType on all of the 4 movie's trees needed:
/// function == INSERT - insert the movie to all of the trees of its genre and none
/// function == REMOVE - remove the movie from all of the trees of its genre and none
/// function == RATE_MOVIE - add rating to the movie in all of his copies in the the trees of its genre and none
/// @param node the movie to do the function on/with
/// @param count the movie's rate or vies to add - in use only for RATE_MOVIE and UPDATE_VIEWS
/// @param function the function type we want to use in this calling of the function
/// @return 
StatusType streaming_database::do_to_all_4_movies_trees(Node<Movie>* node, int count , FunctionType function)
{

	//IdSearch idSearch;
	// Node<Movie>* nodeById = moviesByID[(int)Genre::NONE]->findBy(node, idSearch);

	if(node == NULL)
		return StatusType::INVALID_INPUT;
    try {
        StatusType status1, status2, status3, status4;
        switch (function) {
            case FunctionType::INSERT: {   //TODO: insert or insertBy
                status1 = moviesByRateing[(int) node->get_key().getGenre()]->insert(node->get_key());
                status2 = moviesByRateing[(int) Genre::NONE]->insert(node->get_key());
                status3 = moviesByID[(int) node->get_key().getGenre()]->insert(node->get_key());
                status4 = moviesByID[(int) Genre::NONE]->insert(node->get_key());
                break;
            }
            case FunctionType::REMOVE: {
                status1 = moviesByRateing[(int) node->get_key().getGenre()]->remove(node->get_key());
                status2 = moviesByRateing[(int) Genre::NONE]->remove(node->get_key());
                status3 = moviesByID[(int) node->get_key().getGenre()]->remove(node->get_key());
                status4 = moviesByID[(int) Genre::NONE]->remove(node->get_key());

                break;
            }
            case FunctionType::RATE_MOVIE: {
                if (count < 0 || count > 100)
                    return StatusType::INVALID_INPUT;

                status1 = moviesByRateing[(int) node->get_key().getGenre()]->find(
                        node->get_key())->get_key_by_ref()->add_rating(count);
                status2 = moviesByRateing[(int) node->get_key().getGenre()]->find(
                        node->get_key())->get_key_by_ref()->add_rating(count);
                status3 = moviesByID[(int) node->get_key().getGenre()]->find(
                        node->get_key())->get_key_by_ref()->add_rating(count);
                status4 = moviesByID[(int) node->get_key().getGenre()]->find(
                        node->get_key())->get_key_by_ref()->add_rating(count);

                break;
            }
            case FunctionType::UPDATE_VIEWS: {
                if (count > 0)
                    return StatusType::INVALID_INPUT;

                status1 = moviesByRateing[(int) node->get_key().getGenre()]->find(
                        node->get_key())->get_key_by_ref()->add_views(count);
                status2 = moviesByRateing[(int) node->get_key().getGenre()]->find(
                        node->get_key())->get_key_by_ref()->add_views(count);
                status3 = moviesByID[(int) node->get_key().getGenre()]->find(
                        node->get_key())->get_key_by_ref()->add_views(count);
                status4 = moviesByID[(int) node->get_key().getGenre()]->find(
                        node->get_key())->get_key_by_ref()->add_views(count);

                break;
            }
            default:
                break;
        }

        StatusType ratingStatus = correct_status(status1, status2);
        StatusType iDStatus = correct_status(status3, status4);

        return correct_status(ratingStatus, iDStatus);
    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
}


