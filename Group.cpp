//
// Created by Eva Poluliakhov on 24/05/2023.
//

#include "Group.h"

Group::Group(int id) : groupId(id), isVip(false) {}

/// @brief add new user to a group
/// @param member
void Group::add_user(const User &member) {
    if (member.is_vip()){
        isVip = true;
    }
    members.insert(member);
}

// _________________________________________________GETTERS______________________________________________________________

/// @return how many users this group has.
int Group::get_member_count() {
    return members.get_counter();
}

/// @brief add to a certain genre num of views as the size of the group, after group watch.
/// @param type
void Group::set_views_per_movie(Genre type) {
    viewsAsGroup[(int)type]+= members.get_counter();
}

int Group::get_id() const {
    return groupId;
}
////TODO: operator = for treenode
//void Group::empty_group() {
//    TreeNode<User> toRemove = members;
//    int size = members.get_counter();
//    Node<User>** arr = new Node<User>*[size];
//    toRemove.list_inorder(arr);
//    Node<User>* current;
//    for (int i = 0; i < size; i++){
//        current = arr[i];
//        current->
//    }
//}
//
//Node<User>* Group::empty_group_aux(TreeNode<User>* u){
//    if (u ==NULL) return NULL;
//    empty_group_aux()
//}



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


