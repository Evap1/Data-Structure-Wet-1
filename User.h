//
// Created by Eva Poluliakhov on 24/05/2023.
//

#ifndef WET1_USER_H
#define WET1_USER_H
#include "wet1util.h"

class User{
private:
    int userId;
    bool isVip;
    int viewsPerGenre[(int)Genre::NONE] = {0};
    int groupId;

public:
    explicit User(int userId,bool isVip);

    void setGroupId(int id);
    int getUserId() const;
    bool is_vip() const;
    int getGroupId() const;
    int get_views_per_genre(Genre type) const;
    void add_views_in_genre(Genre type, int nunOfViews);
};
bool operator>(const User &user1, const User &user2);
bool operator<(const User& user1, const User& user2);



#endif //WET1_USER_H
