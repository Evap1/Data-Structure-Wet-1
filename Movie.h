
#ifndef WET1_MOVIE_H
#define WET1_MOVIE_H

#include "wet1util.h"
#include<iostream>
#include "AVL.h"
//
class IdSearch;


class Movie{

    private:
    
    int movieId;
    Genre genre;
    bool vipOnly;
    int views;
    double rate;
    int ratersCont;

public:

    //friend class IdSearch;

    Movie(int id, Genre genreType, int viewsCount, bool isVip);
    ~Movie() =  default;

    int getMovieId() const;
    Genre getGenre() const;
    bool isVipOnly() const;
    int getViews() const;
    double getRate() const;

    StatusType add_views(int count);
    StatusType add_rating(int rated);

    void printMovie() const;


};

bool operator>(const Movie &movie1, const Movie &movie2);
bool operator<(const Movie& movie1, const Movie& movie2);
bool operator==(const Movie &movie1, const Movie &movie2);



class IdSearch{
    public:
    IdSearch() = default;
    bool operator()(const Movie& movie1, const Movie& movie2, Equality kind) const
    {
        if(kind == Equality::LESS)
            return movie1.getMovieId() < movie2.getMovieId();   

        if(kind == Equality::EQUAL)
            return movie1.getMovieId() == movie2.getMovieId();

        //if(kind == Equality::GREATER)
        return movie1.getMovieId() > movie2.getMovieId();
            
    }
};





#endif //WET1_MOVIE_H
