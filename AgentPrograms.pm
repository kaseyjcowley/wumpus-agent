#
#
#
our $aps =
[
 {
   name => 'Simpleton',
   key  => 'S',
   header => 'Simpleton.h',
   cpp    => ['Simpleton.cpp'],
   constructor => 'ai::Sample::Simpleton()',
 },
 {
   name => 'Aristotle',
   key  => 'A',
   header => 'Aristotle.h',
   cpp    => ['Aristotle.cpp'],
   constructor => 'ai::Sample::Aristotle()',
 },
 {
	 name => 'Plato',
	 key => 'P',
	 header => 'Plato.h',
	 cpp => ['kjcPlato.cpp','kjcPlatoModel.cpp', 'WumpusKnowledgeBase.cpp', 'kjcPlatoAction.cpp', 'kjcPlatoState.cpp', 'kjcPlatoProblem.cpp'],
	 constructor => 'kjc::Plato()',
 }
#{
#  name => '',
#  key  => '',
#  header => '',
#  constructor => '',
#},
 ];

1;
