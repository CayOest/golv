#include <golv/games/cards.hpp>
#include <golv/util/cyclic_number.hpp>
#include <golv/games/skat.hpp>
#include <golv/algorithms/mws.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <sstream>

namespace py = pybind11;
using namespace golv;

std::pair<skat::value_type, skat::move_type> mws_binary_search_skat(skat g)
{
  return mws_binary_search(
      g);  // Direkter Aufruf der Template-Spezialisierung für `skat`
}

PYBIND11_MODULE(golv_skat, m)
{
  m.doc() = "Python bindings for the Skat game implementation in C++";

  // suit Enum (Farben)
  py::enum_<suit>(m, "Suit")
      .value("Spades", suit::spades)
      .value("Hearts", suit::hearts)
      .value("Diamonds", suit::diamonds)
      .value("Clubs", suit::clubs)
      .export_values();

  // kind Enum (Kartenwert)
  py::enum_<kind>(m, "Kind")
      .value("Ace", kind::ace)
      .value("King", kind::king)
      .value("Queen", kind::queen)
      .value("Jack", kind::jack)
      .value("Ten", kind::ten)
      .value("Nine", kind::nine)
      .value("Eight", kind::eight)
      .value("Seven", kind::seven)
      .value("Six", kind::six)
      .value("Five", kind::five)
      .value("Four", kind::four)
      .value("Three", kind::three)
      .value("Deuce", kind::deuce)
      .export_values();

  // card Klasse binden
  py::class_<card>(m, "Card")
      .def(py::init<>())              // Standardkonstruktor
      .def(py::init<const char *>())  // Konstruktor mit String
      .def(py::init<kind, suit>())    // Konstruktor mit Kind und Suit
      .def("get_kind", &card::get_kind)
      .def("get_suit", &card::get_suit)
      .def("code", &card::code)
      .def("__repr__", [](const card &c) { return golv::to_string(c); });

  // Bind the skat_card_order struct
  py::class_<skat_card_order>(m, "SkatCardOrder")
      .def(py::init<>())
      .def_readwrite("lead_suit", &skat_card_order::lead_suit)
      .def("__call__", &skat_card_order::operator());

  // Bind the trick struct
  py::class_<skat::trick>(m, "Trick")
      .def(py::init<>())
      .def_readwrite("cards_", &skat::trick::cards_)
      .def_readwrite("leader_", &skat::trick::leader_)
      .def_readwrite("eyes_", &skat::trick::eyes_);

  // Bind the skat class
  py::class_<skat>(m, "Skat")
      .def(py::init<>())  // Standardkonstruktor
      .def("hash_me", &skat::hash_me)
      .def("internal_state", &skat::internal_state)
      .def("legal_actions", &skat::legal_actions)
      .def("value", &skat::value)
      .def("opp_value", &skat::opp_value)
      .def("is_max", &skat::is_max)
      .def("set_soloist", &skat::set_soloist)
      .def("current_player", &skat::current_player)
      .def("apply_action", &skat::apply_action)
      .def("undo_action", &skat::undo_action)
      .def("is_terminal", &skat::is_terminal)
      .def("state", &skat::state)
      .def("deal", &skat::deal)
      .def("tricks", &skat::tricks)
      .def("__repr__", [](const skat &s) {
        std::ostringstream oss;
        oss << s;
        return oss.str();
      });

  // Binding für `mws_binary_search` mit `skat`
  m.def("mws_binary_search", &mws_binary_search_skat,
        "Solves a Skat game using MWS binary search");
}
