#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "cubecoordinate.hh"
#include "hex.hh"
#include "igamerunner.hh"
#include "gameboard.hh"
#include "boardpawn.hh"
#include "boardtransport.hh"
#include "boardhex.hh"
#include "boardpawn.hh"
#include "gamestate.hh"
#include "boardactor.hh"

#include <QMainWindow>
#include <QGraphicsScene>

/**
 * @file
 * @brief Implements MainWindow for the game
 */

namespace Ui {
class MainWindow;
}

namespace Student{

/**
 * @brief Offers a graphical window for the game.
 * QT graphical scene is drawn on a special scenewidget.
 * On this scene are drawn all the graphical objects
 * (hexex, actors, pawns, transports) in the game.
 * Different labels next to the game scene offer information
 * about the ongoing game.
 * @inherits from QMainWindow
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QWidget for MainWindow
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief Destructor
     * @post ui of MainWindow is deleted
     */
    ~MainWindow();

    /**
     * @brief Static function to get MainWindow instance used by the game
     * @return Pointer to the one and only mainwindow instance
     * @note This function can be called from outside of this class
     */
    static MainWindow* getInstance();

    /**
     * @brief getBoardHexMap gets the map of hex graphic pointers
     * @return Map of hex graphic pointers searchable with hex coordinates
     */
    std::map<Common::CubeCoordinate, Ui::BoardHex*> getBoardHexMap();

    /**
     * @brief getBoardPawnMap gets the map of pawn graphic pointers
     * @return Map of pawn graphic pointers searchable with pawn id
     */
    std::unordered_map<int, Ui::BoardPawn*> getBoardPawnMap();

    /**
     * @brief getBoardTransportMap gets the map of transport graphic pointers
     * @return Map of transport graphic pointers searchable with transport id
     */
    std::unordered_map<int, Ui::BoardTransport*> getBoardTransportMap();

    /**
     * @brief getBoardActorMap gets the map of actor graphic pointers
     * @return Map of actor graphic pointers searchable with actor id
     */
    std::unordered_map<int, Ui::BoardActor*> getBoardActorMap();

    /**
     * @brief addBoardTransport creates a graphic transport object
     * @param hex Shared pointer to a common hex object
     * @param boardHex Pointer to a graphical boardHex object
     * @param board Shared pointer to a gameboard object
     * @pre parameter hex points to a hex object
     * @pre parameter boardHex points to a graphical boardhex object
     * @pre paramter board points to a gameboard object
     * @post Creates a transport graphic object
     * @post Adds the object pointer to boardTransportMap
     * @post Exception guarantee: strong
     */
    void addBoardTransport(std::shared_ptr<Common::Hex> hex,
                           Ui::BoardHex* boardHex,
                           std::shared_ptr<Student::GameBoard> board);

    /**
     * @brief addBoardActor creates a graphic actor object
     * @param hex Shared pointer to a common hex object
     * @param boardHex Pointer to a graphical boardHex object
     * @pre parameter hex points to a hex object
     * @pre parameter boardHex points to a graphical boardhex object
     * @post Creates an actor graphic object
     * @post Adds the object pointer to boardActorMap
     * @post Exception guarantee: basic
     */
    void addBoardActor(std::shared_ptr<Common::Hex> hex,
                       Ui::BoardHex* boardHex);

    /**
     * @brief removeBoardPawn removes a graphical pawn object
     * @param id Identification number of the pawn
     * @post Graphical pawn object is removed from the game
     * @post Graphical pawn pointer is removed from boardPawnMap
     * @post Exception quarantee: basic
     */
    void removeBoardPawn(int id);

    /**
     * @brief removeBoardActor removes a graphical actor object
     * @param id Identification number of the actor
     * @post Graphical actor object is removed from the game
     * @post Graphical actor pointer is removed from boardActorMap
     * @post Exception quarantee: basic
     */
    void removeBoardActor(int id);

    /**
     * @brief removeBoardTransport removes a graphical transport object
     * @param id Identification number of the transport
     * @post Graphical transport object is removed from the game
     * @post Graphical transport pointer is removed from boardTransportMap
     * @post Exception quarantee: basic
     */
    void removeBoardTransport(int id);

    /**
     * @brief numberOfGamePhase sets the number of gamephase in ui
     * @post The label value in ui is changed
     * @post Exception quarantee: no throw
     */
    void numberOfGamephase();

    /**
     * @brief numberOfCurrentPlayer sets the number of player in ui
     * @param playerId Id of the current player as int
     * @post The label value in ui is changed
     * @post Exception quarantee: no throw
     */
    void numberOfCurrentPlayer(int playerId);

    /**
     * @brief numberOfActionsLeft sets the number of actions left in ui
     * @param actionsLeft Number of actions left for current player
     * @post The label value in ui is changed
     * @post Exception quarantee: no throw
     */
    void numberOfActionsLeft(int actionsLeft);

    /**
     * @brief updatePointsList updates the pointlist in ui
     * @post The listwidget values in ui are changed
     * @post Exception quarantee: strong
     */
    void updatePointsList();

    /**
     * @brief setGameMessage sets the game message in ui
     * @param msg Message as a string
     * @post The label value in ui is changed
     * @post Exception quarantee: no throw
     */
    void setGameMessage(std::string msg);

    /**
     * @brief getState gets current gamestate
     * @return Shared pointer to gamestate object
     * @post Exception quarantee: no throw
     */
    std::shared_ptr<Student::GameState> getState();

public slots:
    /**
     * @brief numberOfPlayers updates number of players
     * @post Label for number of players in ui is changed
     * @post Exception guarantee: no throw
     */
    void numberOfPlayers(int count);

    /**
     * @brief nextGamephase changes gamephase
     * @post Gamephase is changed and labels in ui are updated
     * @post Exception guarantee: no throw
     */
    void nextGamephase();

private:
    //! Pointer to mainwindow ui
    Ui::MainWindow *ui;

    //! Static pointer to mainwindow instance
    static MainWindow *mainInstance;

    //! Pointer to graphics scene instance
    QGraphicsScene *boardScene;

    //! Shared pointer to igamerunner instance
    std::shared_ptr<Common::IGameRunner> game_;

    //! Map of graphical hex pointers searchable with hex coordinates
    std::map<Common::CubeCoordinate, Ui::BoardHex*> boardHexMap_;

    //! Map of graphical pawn pointers searchable with id
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap_;

    //! Map of graphical transport pointers searchable with id
    std::unordered_map<int, Ui::BoardTransport*> boardTransportMap_;

    //! Map of graphical actor pointers searchable with id
    std::unordered_map<int, Ui::BoardActor*> boardActorMap_;

    //! Count of players in the game
    int playerCount_;

    //! Shared pointer to gamestate instance
    std::shared_ptr<Student::GameState> state_;

    //! Pointer to QMovie object
    QMovie *movie_ = nullptr;

    //! Pointer to movie label
    QLabel *wheelLabel_;

    /**
     * @brief initScene adds needed QT objects for graphics
     * @post Widget, view and scene for graphics are created
     * @post Exception quarantee: strong
     */
    void initScene();

    /**
     * @brief drawhex draws the graphical hex on board
     * @param hex Shared pointer to hex object we are drawing
     * @param board Shared pointer to gameboard object
     * @pre parameter hex points to a hex object
     * @pre paramter board points to a gameboard object
     * @post Hex graphics object is drawn on the screen
     * @post Pawns and transports are added on top of the hex
     * @post Exception quarantee: strong
     */
    void drawHex(std::shared_ptr<Common::Hex> hex,
                 std::shared_ptr<Student::GameBoard> board);

    /**
     * @brief spinWheel spins the wheel
     * @post Label for number of players in ui is changed
     * @post Exception guarantee: no throw
     */
    void spinWheel();

    /**
     * @brief spinWheelMovie creates a QMovie for spinner
     * @post A QMovie instance is created and shown on mainwindow
     * @throw Common::IoException if movie is not valid
     * @post Exception quarantee: basic
     */
    void spinWheelMovie();

};
}


#endif // MAINWINDOW_HH
