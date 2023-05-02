// MapIterator.h

#ifndef MAPITERATOR_H

#define MAPITERATOR_H

template <class Map> class MapRange {
public:
  typedef typename Map::iterator MapIterator;  
  typedef typename Map::const_iterator MapCIterator;  
  struct iterator {
    MapCIterator map_it;
    iterator(MapIterator const &map_it): map_it(map_it) { }
    iterator(MapCIterator const &map_it): map_it(map_it) { }
    MapCIterator operator*() {
      return map_it;
    }
    iterator &operator++() {
      ++map_it;
      return *this;
    }
    bool operator!=(const iterator &other) const {
      return this->map_it != other.map_it;
    }
  };
public:
  MapRange(Map &map): map(map) { }
  iterator begin() {
    return map.begin();
  }
  iterator end() {
    return map.end();
  }
private:
  Map map;
};

template <class Map> MapRange<Map> maprange(Map &map) {
  return MapRange<Map>(map);
};

template <class K, class V> V const &value(QMap<K,V> const &map,
                                           K k, V const &dflt) {
  auto it = map.find(k);
  return (it==map.end()) ? dflt : *it;
}

#endif
