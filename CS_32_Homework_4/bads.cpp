void removeBad(list<Movie*>& li)
{
	list<Movie*>::iterator it;
	for (it = li.begin(); it != li.end(); it++) {
		if ((*it)->rating() < 55) {
			delete* it;
			li.erase(it);
			it = li.begin();
		}
	}
}
