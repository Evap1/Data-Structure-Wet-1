//
// Created by Eva Poluliakhov on 24/05/2023.
//

#include "Group.h"

Group::Group(int id) : groupId(id), isVip(false), members() {}

void Group::add_user(const User &member) {
    if (member.is_vip()){
        isVip = true;
    }
    members.insert(member);
}

int Group::get_member_count() {
    return members.getCounter();
}

// Add to a certain genre num of views as the size of the group, after group watch.
void Group::set_views_per_movie(Genre type) {
    viewsAsGroup[(int)type]+=members.getCounter();
}

int Group::get_id() const {
    return groupId;
}

// Operator overloading
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


