
#include "Movie.h"





//________________________Constructor_________________________

// Consumer must check if ID already exists
Movie::Movie(int id, Genre genreType, int viewsCount, bool isVip) : 
    movieId(id), genre(genreType), views(viewsCount), vipOnly(isVip)
{
    rate = 0;
    ratersCont = 0;
}



 // _______________________________________________Functions______________________________________________

/// @brief add views to the movie's views count
/// @param count assume that count is positive
void Movie::add_views(int count)
{
    if (count >= 0)
        views += count;
}


/// @brief adds one more user's rating to the movie's rating calculation
/// @param rate assume that in range 1-100
void Movie::add_rating(int rated)
{
    if (rated > 0 && rated < 100)
    {
        rate = (ratersCont * rate + rated) / (ratersCont + 1);
        ratersCont++;
    }
}




// _________________________________________________GETTERS______________________________________________________________

int Movie::getMovieId() const{
    return movieId;
}

Genre Movie::getGenre() const{
    return genre;
}

bool Movie::isVipOnly() const{
    return vipOnly;
}

int Movie::getViews() const{
    return views;
}

double Movie::getRate() const{
    return rate;
}





// ___________________________________________Operator Overloading__________________________________________


bool operator>(const Movie &movie1, const Movie &movie2)
{

    if(movie1.getRate() > movie2.getRate())
        return true;
    else if (movie1.getRate() == movie2.getRate())
    {
        if (movie1.getViews() > movie2.getViews())
            return true;
        else if (movie1.getViews() == movie2.getViews())
        {
            if (movie1.getMovieId() > movie2.getMovieId())
                return true;
        }
    }

    return false;
}

bool operator<(const Movie& movie1, const Movie& movie2)
{
    return (movie2 > movie1);
}



