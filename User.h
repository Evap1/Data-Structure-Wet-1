//
// Created by Eva Poluliakhov on 24/05/2023.
//

#ifndef WET1_USER_H
#define WET1_USER_H
#include "wet1util.h"
#include "AVL.h"
#include "Group.h"
class User {
private:
    int userId;
    bool isVip;
    int viewsPerGenre[(int)Genre::NONE] = {0};
    int groupId;
    Group* group;

public:
    explicit User(int userId,bool isVip);
    explicit User(int userId);

    void set_group_id(int id);
    void set_group_ptr(Group* other);

    int get_user_id() const;
    int get_group_id() const;
    Group* get_group_ptr() const;
    int get_views_per_genre(Genre type) const;

    void leave_group();
    bool is_vip() const;
    void add_views_in_genre(Genre type);
    static const int NONE = -1;
};

// ___________________________________________Operator Overloading__________________________________________

bool operator>(const User &user1, const User &user2);
bool operator<(const User& user1, const User& user2);
bool operator==(const User& user1, const User& user2);


#endif //WET1_USER_H
