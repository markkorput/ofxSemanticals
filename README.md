# ofxSemanticals

Semantic classes that wrap around low-level concepts and provides often used functionality.


## Sattribute

This class functions as a *smarter attribute* (though originally the name was short for Super-attribute).

* keeps a history of previous values
* provides events to hook into to (beforeChange, afterChange, beforeSet, afterSet).

### Quick Start

	class Game {
		public: // constructor
			Game(){
				score.set(0);
				ofAddListener(score.afterChange, this, &Game::onScoreChange);
			}

		public: // (s)attributes
			Sattribute<int> score;

		private: // callbacks
			void onScoreChange(Sattribute<int> &sattr){
				int points = attr.get() - attr.prev();
				ofLog() << "You just scored " << ofToString(points) << " point(s)."
			}
	};
