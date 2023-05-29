//
// Created by Eva Poluliakhov on 24/05/2023.
//

#include "Group.h"

Group::Group(int id) : groupId(id), isVip(false) {
    members = new TreeNode<User*>();
}

/// @brief Destructor uses TreeNode destructor
Group::~Group() {
    delete members;
}


// _________________________________________________GETTERS______________________________________________________________

/// @return how many users this group has.
int Group::get_member_count() {
    return members->get_counter();
}

/// @param type
/// @return how many movies the current user watched, of genre type.
int Group::get_views_per_genre(Genre type) const {
    return viewsAsGroup[(int)type];
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
    viewsAsGroup[(int)type]+= members->get_counter();
}

/// @brief keep track of num of views not as group, when already registered in a group.
/// @param type
void Group::set_views_per_movie_user_watch(Genre type) {
    viewsAsGroup[(int)type]++;
}

// _________________________________________________OTHER METHODS______________________________________________________________

/// @brief add new user to a group
/// @param member
StatusType Group::add_user(User* member) {
    if (member->is_vip()){
        isVip = true;
    }
    // add the views per genres each user has watched prior to joining the team.
    for (int i = 0; i < (int)Genre::NONE; i++){
        viewsAsUsers[i] += member->get_views_per_genre((Genre)i);
    }
    member->set_group_ptr(this);
    return members->insert(member);
}


/// @brief Changes all users assigned to this group, groupId to User::NONE,
/// no need to know users, root or so.
void Group::empty_group() {
    Node<User*>* root = members->get_root();
    empty_group_aux(root);
}

//TODO: make sure the warning is ok
/// @brief Recursivly, inorder, change the groupId field in users.
/// @param v - ptr to User* (a.k. node of type User*, so the changes apply in Users tree as well)
void Group::empty_group_aux(Node<User*>* v) {
    if (v == NULL) return;

    empty_group_aux(v->get_right());
    v->get_key_to_set()->leave_group();
    empty_group_aux(v->get_left());
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


