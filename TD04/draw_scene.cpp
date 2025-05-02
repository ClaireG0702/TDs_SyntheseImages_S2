#include "draw_scene.hpp"

/// Camera parameters
float angle_theta {45.0};      // Angle between x axis and viewpoint
float angle_phy {30.0};      // Angle between z axis and viewpoint
float dist_zoom {30.0};      // Distance between origin and viewpoint

GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Set_Of_Points frame{3};

IndexedMesh* sphere;
float lamba;
float ratation_speed = 0.05;

GLBI_Convex_2D_Shape disque{3};
StandardMesh* cone;

GLBI_Convex_2D_Shape line{3};

void initScene() {
	std::vector<float> points {0.0,0.0,0.0};
	somePoints.initSet(points,1.0,1.0,1.0);

	std::vector<float> baseCarre{-10.0,-10.0,0.0,
								10.0,-10.0,0.0,
								10.0,10.0,0.0,
								-10.0,10.0,0.0};
	ground.initShape(baseCarre);
	ground.changeNature(GL_TRIANGLE_FAN);

	std::vector<float> framePoints {
		0.0, 0.0, 0.0,
		10.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 10.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 10.0,
	};
	std::vector<float> frameColors {
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
	};
	frame.initSet(framePoints, frameColors);
	frame.changeNature(GL_LINES);

	// Exercie 2
	sphere = basicSphere();
	sphere->createVAO();

	// Exercice 4
	std::vector<float> disquePoints;
    int numSegments = 50; // Number of segments to approximate the circle
    float angleStep = 2.0f * M_PI / numSegments;
	float z = 0.0;
    for (int i = 0; i < numSegments; ++i) {
        float angle = i * angleStep;
        float x = cos(angle);
        float y = sin(angle);
        disquePoints.push_back(x);
        disquePoints.push_back(y);
		disquePoints.push_back(z);
    }
	disque.initShape(disquePoints);
	disque.changeNature(GL_TRIANGLE_FAN);

	cone = basicCone(1.0, 1.0);
	cone->createVAO();

	std::vector<float> linePoints {
		0.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
	};
	line.initShape(linePoints);
	line.changeNature(GL_LINES);
}

void drawFrame() {
	frame.drawSet();
}

void drawBase() {
	float red = static_cast<float>(235/ 255.f);
	float green = static_cast<float>(207 / 255.f);
	float blue = static_cast<float>(52 / 255.f);
	myEngine.setFlatColor(red, green, blue);

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety({6.0, 6.0, 1.0});
		myEngine.updateMvMatrix();
		disque.drawShape();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addRotation(M_PI / 2, {1.0, 0.0, 0.0});
		myEngine.mvMatrixStack.addHomothety({4.0, 10.0, 4.0});
		myEngine.updateMvMatrix();
		cone->draw();
	myEngine.mvMatrixStack.popMatrix();
}

void drawArm() {
	float red = static_cast<float>(245 / 255.f);
	float green = static_cast<float>(164 / 255.f);	
	float blue = static_cast<float>(66 / 255.f);
	myEngine.setFlatColor(red, green, blue);

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety({3.2, 3.2, 3.2});
		myEngine.updateMvMatrix();
		sphere->draw();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety({2.0, 10.0, 2.0});
		myEngine.updateMvMatrix();
		cone->draw();
		
		myEngine.mvMatrixStack.pushMatrix();
			myEngine.mvMatrixStack.addRotation(M_PI, {0.0, 0.0, 1.0});
			myEngine.updateMvMatrix();
			cone->draw();
	myEngine.mvMatrixStack.popMatrix();
}

void drawPan() {
    // Couleur du disque (plateau)
    float redPlateau = static_cast<float>(89 / 255.f);
    float greenPlateau = static_cast<float>(115 / 255.f);
    float bluePlateau = static_cast<float>(0 / 255.f);
    myEngine.setFlatColor(redPlateau, greenPlateau, bluePlateau);

    // Dessiner le disque
    myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({0.0, 0.0, -5.0});
        myEngine.mvMatrixStack.addHomothety({6.0, 6.0, 1.0}); // Taille du disque
        myEngine.updateMvMatrix();
        disque.drawShape();
    myEngine.mvMatrixStack.popMatrix();

    // Couleur des lignes (rouge)
    myEngine.setFlatColor(1.0, 0.0, 0.0);

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety({1.0, std::sqrt(std::pow(5, 2)+std::pow(3, 2)), 1.0});
		myEngine.mvMatrixStack.addRotation(-M_PI/4, {1.0, 0.0, 0.0});
		myEngine.updateMvMatrix();
		line.drawShape();

		// myEngine.mvMatrixStack.pushMatrix();
		// 	myEngine.mvMatrixStack.addRotation(2 * M_PI / 3.f, {0.0, 0.0, 1.0});
		// 	myEngine.updateMvMatrix();
		// 	line.drawShape();
		// myEngine.mvMatrixStack.popMatrix();
		// myEngine.mvMatrixStack.pushMatrix();
		// 	myEngine.mvMatrixStack.addRotation(-2 * M_PI / 3.f, {0.0, 0.0, 1.0});
		// 	myEngine.updateMvMatrix();
		// 	line.drawShape();
		// myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();

    // Dessiner les trois lignes
    // for (int i = 0; i < 3; ++i) {
    //     float angle = i * (2.0f * M_PI / 3.0f); // Espacement de 2π/3 radians

    //     myEngine.mvMatrixStack.pushMatrix();
    //         // Rotation pour positionner la ligne
    //         myEngine.mvMatrixStack.addRotation(angle, {0.0, 0.0, 1.0});
    //         // Homothétie pour la taille de la ligne
    //         myEngine.mvMatrixStack.addHomothety({0.2, std::sqrt(std::pow(5, 2)+std::pow(3, 2)), 0.2}); // Ligne fine et longue
    //         myEngine.updateMvMatrix();
    //         line.drawShape(); // Utiliser un carré pour représenter la ligne
    //     myEngine.mvMatrixStack.popMatrix();
    // }
}

void drawScene() {
	glPointSize(10.0);

	drawFrame();

	myEngine.setFlatColor(0.2,0.0,0.0);
	ground.drawShape();

	// Exercice 2
	// lamba += ratation_speed;
	// if(lamba > 2 * M_PI) {
	// 	lamba = 0.0;
	// }

	// myEngine.setFlatColor(1.0, 1.0, 0.0);
	// myEngine.mvMatrixStack.pushMatrix();
	// 	myEngine.mvMatrixStack.addRotation(lamba, {0.0, 0.0, 1.0});
	// 	myEngine.mvMatrixStack.addTranslation({4.0, 0.0, 5.0});
	// 	myEngine.mvMatrixStack.addHomothety({3.0, 3.0, 3.0});
	// 	myEngine.updateMvMatrix();
	// 	sphere->draw();
	// myEngine.mvMatrixStack.popMatrix();

	// drawBase();
	// myEngine.mvMatrixStack.pushMatrix();
	// 	myEngine.mvMatrixStack.addTranslation({0.0, 0.0, 10.0});
	// 	myEngine.updateMvMatrix();
	// 	drawArm();
	// myEngine.mvMatrixStack.popMatrix();
	drawPan();
}



