import os
import csv

# Replace this with your target directory path
directory_path = "../../../../SuccessLogs/"

# Initialize the counters
robotMoves = 0
robotManagesToMakeMovement = 0
gripperShouldOpenNum = 0
gripperOpenSuccessNum = 0

gripperPickupOwnPieceSuccessNum = 0
gripperPutDownOwnPieceSuccessNum = 0
gripperPickupDeadPieceSuccessNum = 0
gripperPutdownDeadPieceSuccessNum = 0
gripperPickupEnemyPieceSuccessNum = 0
gripperPutdownEnemyPieceSuccessNum = 0
gripperPickupForCastlingSuccessNum = 0

gripperPickupOwnPieceTotal = 0
gripperPutdownOwnPieceTotal = 0
gripperPickupDeadPieceTotal = 0
gripperPutdownDeadPieceTotal = 0
gripperPickupEnemyPieceTotal = 0



totalMoves = 0
totalPlayerMoves = 0
cameraTotalMoves = 0
robotTotalMoves = 0
totalGripperMoves = 0
cameraFirstTryMove = 0
cameraSecondTryMove = 0
cameraTriedMoreThanTwoTries = 0

angleOfChessBoardBetween5Degrees = 0
angleOfChessBoardBetween5and25Degrees = 0
angleOfChessBoardBetween25and45Degrees = 0
angleOfChessBoardBetween45and90Degrees = 0
angleOfChessBoardBetween90and180Degrees = 0

# Walk through the directory and its subdirectories
for root, dirs, files in os.walk(directory_path):
    for file in files:
        if file.endswith(".csv"):
            file_path = os.path.join(root, file)
            try:
                with open(file_path, 'r', newline='') as csvfile:
                    reader = csv.DictReader(csvfile)
                    for row in reader:
                        try:
                            
                            
                            playerMove = row.get("CameraMove", 0) 
                            if playerMove != "Null": 
                                playerMove = int(playerMove)
                            else:
                                playerMove = 0
                            
                            
                            gripperPickupOwnPieceSuccess = int(row.get("GripperPickupOwnPieceSuccess", 0))
                            gripperPutdownOwnPieceSuccess = int(row.get("GripperPutdownOwnPieceSuccess", 0))
                            gripperShouldPickupDeadPiece = int(row.get("GripperShouldPickupDeadPiece", 0))
                            gripperPickupDeadPieceSuccess = int(row.get("GripperPickupDeadPieceSuccess", 0))
                            gripperPutdownDeadPieceSuccess = int(row.get("GripperPutdownDeadPieceSuccess", 0))
                            gripperShouldPickupEnemyPiece = int(row.get("GripperPickupEnemyPiece", 0))
                            gripperPickupEnemyPieceSuccess = int(row.get("GripperPickupEnemyPieceSuccess", 0))
                            gripperPutdownEnemyPieceSuccess = int(row.get("GripperPutdownEnemyPieceSuccess", 0))
                            gripperPickupForCastling = int(row.get("ExtraPickupForCastling", 0))
                            gripperPickupForCastlingSuccess = int(row.get("ExtraPickupForCastlingSuccess", 0))

                            cameraMoveDepth = row.get("MovesTriedBeforeSuccess", 0)
                            if cameraMoveDepth != "Null":
                                cameraMoveDepth = int(cameraMoveDepth)
                            else:
                                cameraMoveDepth = 0

                            robotManagesToMakeMovement = int(row.get("RobotManagesToMakeMovement", 0))

                            angleOfChessBoard = row.get("AngleOfTransformationMatrixChessboard", 0.0)
                            
                            if angleOfChessBoard != "Null":
                                angleOfChessBoard = float(angleOfChessBoard)
                            else:
                                angleOfChessBoard = -1000.0

                            if playerMove == 1:
                                cameraTotalMoves += 1
                                totalPlayerMoves += 1
                            if playerMove != 1:
                                robotTotalMoves += 1
                            if cameraMoveDepth == 1 and playerMove == 1:
                                cameraFirstTryMove += 1
                            if cameraMoveDepth == 2 and playerMove == 1:
                                cameraSecondTryMove += 1
                            if cameraMoveDepth > 2 and playerMove == 1:
                                cameraTriedMoreThanTwoTries += 1

                            if playerMove != 1 and gripperPickupOwnPieceSuccess == 1:
                                gripperPickupOwnPieceSuccessNum += 1

                            if playerMove == 0:
                                gripperPickupOwnPieceTotal += 1

                            if playerMove != 1 and gripperPutdownOwnPieceSuccess == 1 and gripperPickupOwnPieceSuccess == 1:
                                gripperPutDownOwnPieceSuccessNum += 1
                            
                            if playerMove == 0 and gripperPickupOwnPieceSuccess == 1:
                                gripperPutdownOwnPieceTotal += 1

                            if gripperShouldPickupDeadPiece == 1:
                                gripperPickupDeadPieceTotal += 1
                            
                            if gripperShouldPickupDeadPiece == 1 and gripperPickupDeadPieceSuccess == 1:
                                gripperPickupDeadPieceSuccessNum += 1
                            
                            if gripperShouldPickupDeadPiece == 1 and gripperPutdownDeadPieceSuccess == 1:
                                gripperPutdownDeadPieceSuccessNum += 1

                            if gripperShouldPickupEnemyPiece == 1 and gripperPickupEnemyPieceSuccess:
                                gripperPickupEnemyPieceSuccessNum += 1

                            if gripperShouldPickupEnemyPiece == 1:
                                gripperPickupEnemyPieceTotal += 1

                            if gripperShouldPickupEnemyPiece == 1 and gripperPutdownEnemyPieceSuccess == 1:
                                gripperPutdownEnemyPieceSuccessNum += 1

                            if gripperPickupForCastling == 1 and gripperPickupForCastlingSuccess == 1:
                                gripperPickupOwnPieceSuccessNum += 1
                                gripperPutDownOwnPieceSuccessNum += 1
                                gripperPickupOwnPieceTotal += 1
                                gripperPutdownOwnPieceTotal += 1

                            if playerMove != 1 and robotManagesToMakeMovement == 1:
                                robotMoves += 1
                            
                            if angleOfChessBoard <= 5 and angleOfChessBoard >= -5:
                                angleOfChessBoardBetween5Degrees += 1
                            elif angleOfChessBoard <= 25 and angleOfChessBoard >= -25:
                                angleOfChessBoardBetween5and25Degrees += 1
                            elif angleOfChessBoard <= 45 and angleOfChessBoard >= -45:
                                angleOfChessBoardBetween25and45Degrees += 1
                            elif angleOfChessBoard <= 90 and angleOfChessBoard >= -90:
                                angleOfChessBoardBetween45and90Degrees += 1
                            elif angleOfChessBoard <= 180 and angleOfChessBoard >= -180:
                                angleOfChessBoardBetween90and180Degrees += 1
                                
                            totalMoves += 1
                                
                        except ValueError:
                            print("ValueError encountered, skipping row.")
                            continue
            except Exception as e:
                print(f"Error processing {file_path}: {e}")

print(f"Total moves: {totalMoves}")
print(f"Total robot moves: {robotMoves}")
print(f"Total player moves: {totalPlayerMoves}")
print(f"Camera first try moves: {cameraFirstTryMove}" "/" f"{cameraTotalMoves}")
print(f"Camera second try moves: {cameraSecondTryMove}" "/" f"{cameraTotalMoves}")
print(f"Camera tried more than two tries: {cameraTriedMoreThanTwoTries}" "/" f"{cameraTotalMoves}")
print(f"Angle of chessboard between 5 degrees: {angleOfChessBoardBetween5Degrees}")
print(f"Angle of chessboard between 5 and 25 degrees: {angleOfChessBoardBetween5and25Degrees}")
print(f"Angle of chessboard between 25 and 45 degrees: {angleOfChessBoardBetween25and45Degrees}")
print(f"Angle of chessboard between 45 and 90 degrees: {angleOfChessBoardBetween45and90Degrees}")
print(f"Angle of chessboard between 90 and 180 degrees: {angleOfChessBoardBetween90and180Degrees}")
print(f"Gripper pickup own piece success: {gripperPickupOwnPieceSuccessNum}" "/" f"{gripperPickupOwnPieceTotal}")
print(f"Gripper put down own piece success: {gripperPutDownOwnPieceSuccessNum}" "/" f"{gripperPutdownOwnPieceTotal}")
print(f"Gripper pickup dead piece success: {gripperPickupDeadPieceSuccessNum}" "/" f"{gripperPickupDeadPieceTotal}")
print(f"Gripper put down dead piece success: {gripperPutdownDeadPieceSuccessNum}" "/" f"{gripperPickupDeadPieceTotal}")
print(f"Gripper pickup enemy piece success: {gripperPickupEnemyPieceSuccessNum}" "/" f"{gripperPickupEnemyPieceTotal}")
print(f"Gripper put down enemy piece success: {gripperPutdownEnemyPieceSuccessNum}" "/" f"{gripperPickupEnemyPieceTotal}")
