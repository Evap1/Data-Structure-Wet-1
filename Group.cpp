//
// Created by Eva Poluliakhov on 24/05/2023.
//

#include "Group.h"

Group::Group(int id) :  groupId(id), isVip(false) ,numOfVIP(0) , isTemp(true){
    members = new TreeNode<User*>();
//    numOfVIP =0;

}


/// @brief Destructor uses TreeNode destructor
Group::~Group() {
    if(!isTemp)
    {
        if (members->get_root() != NULL && members->get_counter()>0){
            Node<User*>* root = members->get_root();
            empty_group_aux(root);
        }
    }
    delete members;
}

// _________________________________________________GETTERS______________________________________________________________

/// @return how many users this group has.
//TODO: if do problems, change with simple counter.
int Group::get_member_count() {
    return members->get_counter();
}

/// @param type
/// @return how many views the current group has, of genre type.
int Group::get_views_per_genre(Genre type) const {
    return sumViewsAsGroup[(int)type];
}
/// @param type
/// @return how many movies the current group watched, of genre type.
int Group::get_movies_as_group(Genre type) const {
    return numOfMoviesWatched[(int)type];
}

/// @return how many users this group has.
bool Group::is_vip() {
    return isVip;
}

int Group::get_id() const {
    return groupId;
}

// _________________________________________________SETTERS______________________________________________________________


/// @brief add to a certain genre num of views as the size of the group, after group watch.
/// @param type
void Group::set_views_per_movie(Genre type) {
    sumViewsAsGroup[(int)type]+= members->get_counter();
}

/// @brief keep track of num of views not as group, when already registered in a group.
/// @param type
void Group::set_views_per_movie_user_watch(Genre type) {
    sumViewsAsGroup[(int)type]++;
}

void Group::set_views(Genre genre, int amount){
    sumViewsAsGroup[(int)genre] += amount;
}

/// in use only at add group!! and only in use when the group is temporaty
/// \param ptr
void Group::set_members(TreeNode<User*>* ptr){
    if(ptr == NULL)
    {
        if(isTemp)
        {
            members = ptr;
        }
    }
}

// _________________________________________________OTHER METHODS______________________________________________________________

/// @brief add new user to a group
/// @param member
StatusType Group::add_user(User* member, UserPtrCompare ptrCompare) {
    if(member == NULL)
        return StatusType::FAILURE;

    if(isTemp)
        isTemp = false;

    if (member->is_vip()){
        isVip = true;
        numOfVIP++;
    }
    // add the views per genres each user has watched prior to joining the team.
    for (int i = 0; i <= (int)Genre::NONE; i++){
        sumViewsAsGroup[i] += member->get_views_per_genre((Genre)i);
    }
    // update how many movies the team watched of type genre.
    for (int i = 0; i <= (int)Genre::NONE; i++){
        member->set_views_as_group_join((Genre)i,numOfMoviesWatched[i] );
    }
    //TODO: maybe problems with ptr
    member->set_group_ptr(this);
    return members->insertBy(member, ptrCompare);
}

//TODO: make sure the warning is ok
/// @brief Recursivly, inorder, change the groupId field in users,
/// @param v - ptr to User* (a.k. node of type User*, so the changes apply in Users tree as well)
void Group::empty_group_aux( Node<User*>* v) {
    if (v == NULL) return;

    // recursive call to left
    empty_group_aux(v->get_left_nonConst());

    // action on the element
    for (int i = 0; i <= (int)Genre::NONE; i++){
        int toAdd = v->get_key_to_set()->get_views_when_leave((Genre)i);
        v->get_key_to_set()->set_views_as_group_join((Genre)i, toAdd);
    }
    v->get_key_to_set()->set_group_id(User::NONE);
    //TODO: maybe problems with null
    v->get_key_to_set()->set_group_ptr_null(nullptr);

    // recursive call to right
    empty_group_aux(v->get_right_nonConst());
}

StatusType Group::free_members() {
    StatusType status = members->delete_tree();
    if (status == StatusType::SUCCESS){
        if (members->get_root()==NULL){
            return StatusType::SUCCESS;
        }
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::FAILURE;
}

void Group::remove_vip()
{
    numOfVIP--;
    if(numOfVIP==0)
        isVip = false;
}



// ___________________________________________Operator Overloading__________________________________________

bool operator>(const Group &group1, const Group &group2)
{
    return (group1.get_id() > group2.get_id());
}

bool operator<(const Group& group1, const Group& group2)
{
    return (group2 > group1);
}

bool operator==(const Group& group1, const Group& group2)
{
    return (group1.get_id() == group2.get_id());
}


