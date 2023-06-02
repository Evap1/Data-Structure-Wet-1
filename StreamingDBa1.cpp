#include "StreamingDBa1.h"
#include "AVL.h"
#include <new>

StatusType correct_status(StatusType status1,StatusType status2);

//TODO: may cause problems with new and mempry dealing. users..
/// @brief Constructor for an empty streaming database.
// no need to free elements alloced before, not dynamiclly.
streaming_database::streaming_database()
{
    usersPtr = std::unique_ptr<TreeNode<User>>(new TreeNode<User>());
    groupsPtr = std::unique_ptr<TreeNode<Group>>(new TreeNode<Group>());

    //usersPtr = std::make_unique<TreeNode<User>>(TreeNode<User>());
//    groupsPtr = std::make_unique<TreeNode<Group>>(TreeNode<Group>());
    users = *usersPtr;
    groups = *groupsPtr;

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
//		if (bestMovie[i] != nullptr){
//			delete bestMovie[i];
//		}
    }
    //deleted and their info as they are unique ptrs
//    delete groupsPtr;
//    delete usersPtr;
}

/// @brief add movie by id to all 4 trees. O(logk)
/// @param movieId
/// @param genre
/// @param views
/// @param vipOnly
/// @return Status of the operation
StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
///TODO: add try catch to all of this function
    IdSearch idSearch;

    if(movieId <= 0 || genre == Genre::NONE ||  views < 0)
        return StatusType::INVALID_INPUT;

    Node<Movie>* node = new Node<Movie>(Movie(movieId,genre,views, vipOnly));
    if(moviesByID[(int)Genre::NONE]->findBy(node->get_key(), idSearch) == NULL)
    {
        StatusType status = do_to_all_4_movies_trees(node,-1,FunctionType::INSERT);
        delete node;
        return status;
    }
    delete node;
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
        Node<Movie>* node = moviesByID[(int)Genre::NONE]->findBy(temp->get_key(), idSearch);
        delete temp;
        if(node != NULL)
        {
            return do_to_all_4_movies_trees(node, -1, FunctionType::REMOVE);
        }
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
    User* user = new User(userId,isVip);

    StatusType status = users.insert(*user);
    delete user;
    return status;
}

/// @brief remove existing user from users tree. O(logn)
/// @param userId
/// @return Status of the operation
StatusType streaming_database::remove_user(int userId)
{
    if (userId <= 0) return StatusType::INVALID_INPUT;
    Node<User>* toRemove = users.find(User(userId));
    if(toRemove== NULL)
        return StatusType::FAILURE;

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
        if(toRemove->get_key_by_ref()->is_vip())
            toUpdate->remove_vip();

        UserPtrCompare ptrCompare;
        toUpdate->get_members()->removeBy(toRemove->get_key_by_ref(), ptrCompare);
    }
    return users.remove(User(userId));
}

/// @brief add an empty new group to groups tree. O(logm)
/// @param groupId
/// @return Status of the operation
StatusType streaming_database::add_group(int groupId)
{
    if (groupId <= 0) return StatusType::INVALID_INPUT;
    Group* group = new Group(groupId, true);
    StatusType status = groups.insert(*group);
//    group->set_members(NULL);
    delete group;
    return status;

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

    // find group in the tree. if not found, it's a failure
    Node<Group>* toDelete = groups.find(Group(groupId, true));
    if (toDelete == NULL) return StatusType::FAILURE;

    ///TODO: check agin the logic and what we want to do here!
    /// if we take it' we need to change the Group distructor - to free members only
    //    if (toDelete->get_key_by_ref()->get_members()->get_root() != NULL && toDelete->get_key_by_ref()->get_members()->get_counter()>0) {
    //    Node<User *> *root = toDelete->get_key_by_ref()->get_members()->get_root();
    //    if (root != NULL)
    //       toDelete->get_key_by_ref()->empty_group_aux(root);


    //toDelete->get_key_by_ref()->empty_group();
    return groups.remove(Group(groupId, true));
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
    Node<Group>* NodeGroupToUpdate = groups.find(Group(groupId,true));
    if (NodeGroupToUpdate == NULL) return StatusType::FAILURE;

    // find ptr to user to add. if not found, it's a failure
    User* UserToAdd = users.find(User(userId))->get_key_by_ref();
    if (UserToAdd == NULL) return StatusType::FAILURE;

    // if user has group, it's a failure. otherwise; update the group id of the user
    int UserGroup = UserToAdd->get_group_id();
    if (UserGroup != User::NONE) return StatusType::FAILURE;
    UserToAdd->set_group_id(groupId);

    UserPtrCompare ptrCompare;
    // add user to the current group tree and fill the movies viewed as group prior to join
    StatusType status =NodeGroupToUpdate->get_key_to_set().add_user(UserToAdd, ptrCompare);
    NodeGroupToUpdate = nullptr;
    return status;
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
    Node<Movie>* movieNodeToUpdate = moviesByID[(int)Genre::NONE]->findBy(tempMovieNode->get_key(), idSearch);
    delete tempMovieNode;
    // movie not found
    if (movieNodeToUpdate == NULL) {
        return StatusType::FAILURE;
    }

    Movie *MovieToUpdate = movieNodeToUpdate->get_key_by_ref();

    // check if movie is vip and user allowed to watch
    // case 1: user not allowed:
    if (MovieToUpdate->isVipOnly() && !UserToAdd->is_vip()) {
        return StatusType::FAILURE;
    }

    // case 2: user allowed:
    StatusType tempStatus = do_to_all_4_movies_trees(movieNodeToUpdate, 1, FunctionType::UPDATE_VIEWS);
    if(tempStatus != StatusType::SUCCESS){
        return tempStatus;
    }
    // update views for user
    UserToAdd->add_views_in_genre(MovieToUpdate->getGenre());

    // case 2.1: if related to a group, want to add views to the group as individual.
    if (UserToAdd->get_group_id() != User::NONE){
        Group* GroupToAdd = UserToAdd->get_group_ptr();
        GroupToAdd->set_views_per_movie_user_watch(MovieToUpdate->getGenre());
    }
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
    Group *GroupToAdd = groups.find(Group(groupId,true))->get_key_by_ref();
    if (GroupToAdd == NULL || GroupToAdd->get_member_count() == 0) return StatusType::FAILURE;

    // find ptr to user to movie. if not found, it's a failure
    //TODO: add find by id, in NONE tree.
    IdSearch idSearch;
    Node<Movie>* tempMovieNode = new Node<Movie>(Movie(movieId,Genre::NONE,0, false));
    Node<Movie>* movieNodeToUpdate = moviesByID[(int)Genre::NONE]->findBy(tempMovieNode->get_key(), idSearch);
    delete tempMovieNode;

    if (movieNodeToUpdate == NULL) {
        return StatusType::FAILURE;
    }
    Movie *MovieToUpdate = movieNodeToUpdate->get_key_by_ref();

    // check if movie is vip and user allowed to watch
    // user not allowed
    if (MovieToUpdate->isVipOnly() && !GroupToAdd->is_vip()) {
        return StatusType::FAILURE;
    }
    // TODO: update the counter in all 4 trees
    int viewsCount = GroupToAdd->get_member_count();
    do_to_all_4_movies_trees(movieNodeToUpdate, viewsCount, FunctionType::UPDATE_VIEWS);

    // update views for group
    GroupToAdd->set_views_per_movie(MovieToUpdate->getGenre());

    // TODO: calculate the status
    return StatusType::SUCCESS;
}

/// @brief method to get number of movies in database per genre. O(1)
/// @param genre
/// @return status and number of movies
output_t<int> streaming_database::get_all_movies_count(Genre genre)
{

    if(moviesByRateing[(int)genre] != NULL && moviesByRateing[(int)genre]->get_root() != NULL)
    {
//		output_t<int>* temp = new output_t<int>(moviesByRateing[(int)genre]->get_counter());
//			return *temp;
        return {output_t<int>(moviesByRateing[(int)genre]->get_counter())};


    }
//	output_t<int>* temp = new output_t<int>(0);
//	return *temp;
    return {output_t<int>(0)};
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
        get_all_movies_inside(moviesByRateing[(int)genre]->get_root(), output,0);
        return StatusType::SUCCESS;
    }
    return StatusType::FAILURE;
}

/// Inside private function!
/// @brief An inside functuion that recursively puts the movies ID's in output array by preorder sort
/// @param moviesTree
/// @param output
/// @return The return is for the rcursion for the indexing in the array

int streaming_database::get_all_movies_inside(const Node<Movie>* moviesRoot, int *const output, int startIndex)
{

    /// TODO: create a get left and get right function to the tree
    if (moviesRoot == NULL){
        return startIndex;
    }

    startIndex = get_all_movies_inside(moviesRoot->get_right() , output, startIndex);
    output[startIndex] = moviesRoot->get_key().getMovieId();
    startIndex++;
    return get_all_movies_inside(moviesRoot->get_left() , output, startIndex);
//    return startIndex;

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
    if((users.get_root() == NULL) || (userNode == NULL))
        return StatusType::FAILURE;

    //access the whole tree sorted by rate
    if(moviesByRateing[(int)Genre::NONE] != NULL && moviesByRateing[(int)Genre::NONE]->get_root() != NULL)
    {
        IdSearch idSearch;

        //find movie by rate
        Node<Movie> *temp = new Node<Movie>(Movie(movieId,Genre::NONE, 0, false));
        Node<Movie>* movieNode = moviesByID[(int)Genre::NONE]->findBy(temp->get_key(), idSearch);
        delete temp;
        if(movieNode == NULL)
            return StatusType::FAILURE;

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
    Group* GroupToAdd = groups.find(Group(groupId,true))->get_key_by_ref();

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
    if (bestMovie[(int)favourite] == NULL){
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



    if(node == NULL)
        return StatusType::INVALID_INPUT;
    if(node->get_key_by_ref() == NULL)
        return StatusType::INVALID_INPUT;

    Genre genreToUpdate = node->get_key().getGenre();
    IdSearch idSearch;
    try {
        StatusType status1, status2, status3, status4;
        switch (function) {
            case FunctionType::INSERT: {   //TODO: insert or insertBy
                status1 = moviesByRateing[(int) genreToUpdate]->insert(node->get_key());
                status2 = moviesByRateing[(int) Genre::NONE]->insert(node->get_key());
                status3 = moviesByID[(int) genreToUpdate]->insertBy(node->get_key(), idSearch);
                status4 = moviesByID[(int) Genre::NONE]->insertBy(node->get_key(), idSearch);
                break;
            }
            case FunctionType::REMOVE: {
                // pay attention that node is a pinter to a movie node from the tree: moviesByID[NONE]
                // so it will be deleted when this node is deleted at theis tree
                status1 = moviesByRateing[(int) genreToUpdate]->remove(node->get_key());
                status2 = moviesByRateing[(int) Genre::NONE]->remove(node->get_key());
                status3 = moviesByID[(int) genreToUpdate]->removeBy(node->get_key(), idSearch);
                status4 = moviesByID[(int) Genre::NONE]->removeBy(node->get_key(),idSearch);

                break;
            }
            case FunctionType::RATE_MOVIE: {
                if (count < 0 || count > 100)
                    return StatusType::INVALID_INPUT;

                // Node<Movie>* nodeToFunc = new Node(*moviesByID[(int)Genre::NONE]->findBy(node->get_key(), idSearch));

                status1 = moviesByRateing[(int) genreToUpdate]->find(node->get_key())
                        ->get_key_by_ref()->add_rating(count);
                status2 = moviesByRateing[(int) Genre::NONE]->find(node->get_key())
                        ->get_key_by_ref()->add_rating(count);
                status3 = moviesByID[(int) genreToUpdate]->findBy(node->get_key(), idSearch)
                        ->get_key_by_ref()->add_rating(count);
                status4 = moviesByID[(int) Genre::NONE]->findBy(node->get_key(), idSearch)
                        ->get_key_by_ref()->add_rating(count);

//                delete nodeToFunc;
                break;
            }
            case FunctionType::UPDATE_VIEWS: {
                if (count <= 0)
                    return StatusType::INVALID_INPUT;

//                Node<Movie>* nodeToFunc = new Node(*moviesByID[(int)Genre::NONE]->findBy(node->get_key(), idSearch));

                status1 = moviesByRateing[(int) genreToUpdate]->find(node->get_key())
                        ->get_key_by_ref()->add_views(count);
                status2 = moviesByRateing[(int) Genre::NONE]->find(node->get_key())
                        ->get_key_by_ref()->add_views(count);
                status3 = moviesByID[(int) genreToUpdate]->findBy(node->get_key(),idSearch)
                        ->get_key_by_ref()->add_views(count);
                status4 = moviesByID[(int) Genre::NONE]->findBy(node->get_key(),idSearch)
                        ->get_key_by_ref()->add_views(count);

//                delete nodeToFunc;
                break;
            }
            default:
                break;
        }

        //update best movie;
        bestMovie[(int) genreToUpdate] = moviesByRateing[(int)genreToUpdate]->findMax();
        bestMovie[(int) Genre::NONE] = moviesByRateing[(int)Genre::NONE]->findMax();

        StatusType ratingStatus = correct_status(status1, status2);
        StatusType iDStatus = correct_status(status3, status4);

        return correct_status(ratingStatus, iDStatus);
    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
}
