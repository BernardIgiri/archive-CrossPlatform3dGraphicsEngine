#pragma once

namespace algs
{
	/*
		Split(begin,end,delimiter,dest,compress)
		Pre-condition:
			- begin and end are iterators to the same STL compliant sequence
			- delimiter contains the value at which the input sequence should be split at
			- dest is an output iterator that can recieve the split results of begin - end
			- the value type of the dest iterator has a range constructor that accepts a range from begin - end
			- compress is set to true if empty ranges between delimiters are to be skipped
		Post-condition:
			- dest is filled with the resulting sections of begin - end that seperated by delimiter
		Usage:
			std::string input="Test, of, split";
			std::vector<std::string> output;
			Split(input.begin(),input.end(),std::back_inserter(output),true);
	*/

	// Splits string s at delimiter placing resultant strings into output iterator dest
	template<typename InputIterator, typename OutputIterator>
	void Split( InputIterator begin
			  , InputIterator end
			  , typename std::iterator_traits<InputIterator>::value_type delimiter
			  , OutputIterator dest
			  , bool compress=true
			  )
	{
		typedef typename std::iterator_traits<InputIterator>::value_type value_type;
		typedef typename std::iterator_traits<OutputIterator>::value_type out_type;

		InputIterator buffer_start = begin;
		InputIterator buffer_end = begin;
		for (InputIterator i = begin; i != end; ++i)
		{
			if (*i != delimiter)
			{
				++buffer_end;
			}
			else if (buffer_start != buffer_end || !compress)
			{
				*dest = out_type(buffer_start, buffer_end);
				++dest;
				buffer_start = i;
				buffer_end = i;
			}
		}

		if (buffer_start != buffer_end || !compress)
		{
			*dest = out_type(buffer_start, buffer_end);
		}
	};

	/*
		unstable_remove_if(first,last,pred)
		Pre-condition:
			- first and last are iterators to the same STL compliant sequence
			- swap can be safely applied on the values within the range of first - last
			- pred(*first) returns true only for each item in first - last that should be removed
		Post-condition:
			- the values within first - last have been resorted so as to ensure that all values
				between first and the returned iterator are values that evaluated false
				for pred(value).
			- The original order of the container is not preserved
		Usage:
			unstable_remove_if(c.begin(),c.end(),std::bind2nd(std::equal_to<T>(), value));
	*/

	// faster than remove_if at the cost of not preserving order
	template <typename ForwardItor, typename Pred>
	ForwardItor unstable_remove_if(ForwardItor first, ForwardItor last, Pred pred)
	{
		while(first != last)
		{
			if(pred(*first))
				swap(*first, *--last);
			else
				++first;
		}
		return last;
	}

	/*
		unstable_remove(first,last,pred)
		Pre-condition:
			- first and last are iterators to the same STL compliant sequence
			- swap can be safely applied on the values within the range of first - last
			- value compares equal to all values which should be removed from first - last
		Post-condition:
			- the values within first - last have been resorted so as to ensure that all values
				between first and the returned iterator are values that evaluated to equal
				value.
			- The original order of the container is not preserved
		Usage:
			unstable_remove(c.begin(),c.end(),value);
	*/

	// faster than remove at the cost of not preserving order
	template <typename ForwardItor, typename T>
	ForwardItor unstable_remove(ForwardItor first, ForwardItor last, const T &value)
	{
		return unstable_remove_if(first, last, std::bind2nd(std::equal_to<T>(), value));
	}

	// predicate for comparing only the first item in an std::pair
	template <typename PairT>
	bool LessFirst(PairT left,PairT right) { return left.first<right.first; };
	// predicate for comparing only the first item in an std::pair
	template <typename PairT>
	bool EqualFirst(PairT left,PairT right) { return left.first==right.first; };
	// predicate for comparing only the second item in an std::pair
	template <typename PairT>
	bool LessSecond(PairT left,PairT right) { return left.second<right.second; };
	// predicate for comparing only the second item in an std::pair
	template <typename PairT>
	bool EqualSecond(PairT left,PairT right) { return left.second==right.second; };

	// returns the number of elements in a C style array
	template <typename T>
	size_t ArraySize(T a[]) { return sizeof(a)/sizeof(T); };
	// returns the end iterator to a C style array
	template <typename T>
	inline T *ArrayEnd(T a[]) { return a+ArraySize(a); };
};
