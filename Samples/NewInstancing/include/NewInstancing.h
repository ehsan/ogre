#ifndef __NewInstancing_H__
#define __NewInstancing_H__

#include "SdkSample.h"
#include "OgreInstancedEntity.h"

using namespace Ogre;
using namespace OgreBites;

static const char *c_instancingTechniques[] =
{
	"Shader Based",
	"Vertex Texture Fetch (VTF)",
	"No Instancing"
};

static const char *c_materialsTechniques[] =
{
	"Examples/Instancing/ShaderBased/Robot",
	"Examples/Instancing/VTF/Robot",
	"Examples/Instancing/ShaderBased/Robot"
};

static const char *c_meshNames[] =
{
	"robot.mesh"
};

#define NUM_TECHNIQUES 3

class _OgreSampleClassExport Sample_NewInstancing : public SdkSample
{
public:

	Sample_NewInstancing() : NUM_INST_ROW(50), NUM_INST_COLUMN(50)
	{
		mInfo["Title"] = "New Instancing";
		mInfo["Description"] = "Demonstrates how to use the new InstancedManager to setup many dynamic"
			"instances of the same mesh with much less performance impact";
		mInfo["Thumbnail"] = "thumb_grass.png";
		mInfo["Category"] = "Environment";
		mInfo["Help"] = "Press Space to switch Instancing Techniques.\n"
						"Press B to toggle bounding boxes";
	}

    bool frameRenderingQueued(const FrameEvent& evt)
    {
		if( mAnimateInstances->isChecked() )
			animateUnits( evt.timeSinceLastEvent );

		if( mMoveInstances->isChecked() )
			moveUnits( evt.timeSinceLastEvent );

		return SdkSample::frameRenderingQueued(evt);        // don't forget the parent class updates!
    }

	bool keyPressed(const OIS::KeyEvent& evt)
	{
		// toggle bounding boxes with B key unless the help dialog is visible
		if (evt.key == OIS::KC_B && !mTrayMgr->isDialogVisible())
			mSceneMgr->showBoundingBoxes( !mSceneMgr->getShowBoundingBoxes() );

		/*if (evt.key == OIS::KC_SPACE && !mTrayMgr->isDialogVisible())
		{
			clearScene();
			switchInstancingTechnique();
		}*/

		return SdkSample::keyPressed(evt);
	}

protected:
	void setupContent()
	{
		mEntities.reserve( NUM_INST_ROW * NUM_INST_COLUMN );
		mSceneNodes.reserve( NUM_INST_ROW * NUM_INST_COLUMN );
		mAnimations.reserve( NUM_INST_ROW * NUM_INST_COLUMN );

		mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");

		// create a mesh for our ground
		MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Vector3::UNIT_Y, 0), 1000, 1000, 20, 20, true, 1, 6, 6, Vector3::UNIT_Z);
		
		// create a ground entity from our mesh and attach it to the origin
		Entity* ground = mSceneMgr->createEntity("Ground", "ground");
		ground->setMaterialName("Examples/NewInstancingFloor");
		ground->setCastShadows(false);
		mSceneMgr->getRootSceneNode()->attachObject(ground);

		setupLighting();

		// set initial camera position and speed
		mCamera->setPosition( 0, 50, 100 );

		setupGUI();

#if OGRE_PLATFORM != OGRE_PLATFORM_IPHONE
		setDragLook(true);
#endif

		//Initialize the techniques and current mesh
		mInstancingTechnique	= 0;
		mCurrentMesh			= 0;
		mCurrentManager			= 0;
		for( int i=0; i<NUM_TECHNIQUES-1; ++i )
			mInstanceManagers[i] = 0;
		switchInstancingTechnique();
	}

	void setupLighting()
	{
		mSceneMgr->setAmbientLight( ColourValue( 0.25f, 0.25f, 0.25f ) );

		ColourValue lightColour( 1, 0.5, 0.3 );

		// create a light
		Light* light = mSceneMgr->createLight();
		light->setDiffuseColour(lightColour);
		light->setPosition( 0.0f, 25.0f, 0.0f );
		light->setSpecularColour( 0.6, 0.82, 1.0 );
		light->setAttenuation( 3500, 0.085, 0.00008, 0.00006);
	}

	void switchInstancingTechnique()
	{
		//mInstancingTechnique = (mInstancingTechnique+1) % NUM_TECHNIQUES;
		mInstancingTechnique = mTechniqueMenu->getSelectionIndex();

		if( mInstancingTechnique < NUM_TECHNIQUES-1 )
		{
			//Instancing

			//Create the manager if we haven't already (i.e. first time)
			//Because we use IM_USEALL as flags, the actual num of instances per batch might be much lower
			//If you're not bandwidth limited, you may want to lift IM_VTFBESTFIT flag away
			if( !mInstanceManagers[mInstancingTechnique] )
			{
				InstanceManager::InstancingTechnique technique;
				switch( mInstancingTechnique )
				{
				case 0: technique = InstanceManager::ShaderBased; break;
				case 1: technique = InstanceManager::TextureVTF; break;
				}

				mInstanceManagers[mInstancingTechnique] = mSceneMgr->createInstanceManager(
				"InstanceMgr" + StringConverter::toString(mInstancingTechnique), c_meshNames[mCurrentMesh],
				ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, technique,
				NUM_INST_ROW * NUM_INST_COLUMN, IM_USEALL );
			}

			mCurrentManager = mInstanceManagers[mInstancingTechnique];

			createInstancedEntities();

			//Show GUI features available only to instancing
			mDefragmentBatches->show();
			mDefragmentOptimumCull->show();
		}
		else
		{
			//Non-instancing
			createEntities();

			//Hide GUI features available only to instancing
			mCurrentManager = 0;
			mDefragmentBatches->hide();
			mDefragmentOptimumCull->hide();
		}

		createSceneNodes();
	}

	void createEntities()
	{
		for( int i=0; i<NUM_INST_ROW * NUM_INST_COLUMN; ++i )
		{
			//Create the non-instanced entity. Use the same shader as shader-based because:
			//a. To prove we can (runs without modification! :-) )
			//b. Make a fair comparision
			Entity *ent = mSceneMgr->createEntity( c_meshNames[mCurrentMesh] );
			ent->setMaterialName( c_materialsTechniques[NUM_TECHNIQUES-1] );
			mEntities.push_back( ent );

			//Get the animation
			AnimationState *anim = ent->getAnimationState( "Walk" );
			anim->setEnabled( true );
			anim->addTime( i * i * i * 0.001f  ); //Random start offset
			mAnimations.push_back( anim );
		}
	}

	void createInstancedEntities()
	{
		for( int i=0; i<NUM_INST_ROW * NUM_INST_COLUMN; ++i )
		{
			//Create the instanced entity
			InstancedEntity *ent = mCurrentManager->createInstancedEntity(
												c_materialsTechniques[mInstancingTechnique] );
			mEntities.push_back( ent );

			//Get the animation
			AnimationState *anim = ent->getAnimationState( "Walk" );
			anim->setEnabled( true );
			anim->addTime( i * i * i * 0.001f  ); //Random start offset
			mAnimations.push_back( anim );
		}
	}

	void createSceneNodes()
	{
		//Here the SceneNodes are created. Since InstancedEntities derive from MovableObject,
		//they behave like regular Entities on this.
		SceneNode *rootNode = mSceneMgr->getRootSceneNode();

		for( int i=0; i<NUM_INST_ROW; ++i )
		{
			for( int j=0; j<NUM_INST_COLUMN; ++j )
			{
				int idx = i * NUM_INST_COLUMN + j;
				SceneNode *sceneNode = rootNode->createChildSceneNode();
				sceneNode->attachObject( mEntities[idx] );
				sceneNode->setScale( Vector3( 0.1f ) );
				sceneNode->yaw( Radian( (i+1) * (j+1) * (i+1) * (j+1) ) ); //Random orientation
				sceneNode->setPosition( mEntities[idx]->getBoundingRadius() * (i - NUM_INST_ROW * 0.5f), 0,
										mEntities[idx]->getBoundingRadius() * (j - NUM_INST_COLUMN * 0.5f) );

				mSceneNodes.push_back( sceneNode );
			}
		}
	}

	void clearScene()
	{
		std::vector<MovableObject*>::const_iterator itor = mEntities.begin();
		std::vector<MovableObject*>::const_iterator end  = mEntities.end();

		//Note: Destroying the instance manager automatically destroys all instanced entities
		//created by this manager (beware of not leaving reference to those pointers)
		while( itor != end )
		{
			SceneNode *sceneNode = (*itor)->getParentSceneNode();
			sceneNode->detachAllObjects();
			sceneNode->getParentSceneNode()->removeAndDestroyChild( sceneNode->getName() );

			if( mInstancingTechnique == NUM_TECHNIQUES-1 )
				mSceneMgr->destroyEntity( (*itor)->getName() );
			else
				mSceneMgr->destroyInstancedEntity( static_cast<InstancedEntity*>(*itor) );

			++itor;
		}

		mEntities.clear();
		mSceneNodes.clear();
		mAnimations.clear();
	}

	void destroyManagers()
	{
		for( int i=0; i<NUM_TECHNIQUES-1; ++i )
		{
			if( mInstanceManagers[i] )
			{
				mSceneMgr->destroyInstanceManager( mInstanceManagers[i] );
				mInstanceManagers[i] = 0;
			}
		}
	}

	void cleanupContent()
	{
		clearScene();
		destroyManagers();
	}

	void animateUnits( float timeSinceLast )
	{
		//Iterates through all AnimationSets and updates the animation being played. Demonstrates the
		//animation is unique and independent to each instance
		std::vector<AnimationState*>::const_iterator itor = mAnimations.begin();
		std::vector<AnimationState*>::const_iterator end  = mAnimations.end();

		while( itor != end )
		{
			(*itor)->addTime( timeSinceLast );
			++itor;
		}
	}

	void moveUnits( float timeSinceLast )
	{
		const Real fMovSpeed = mEntities[0]->getBoundingRadius() * 0.25f;

		//Randomly move the units along their normal, bouncing around invisible walls
		std::vector<SceneNode*>::const_iterator itor = mSceneNodes.begin();
		std::vector<SceneNode*>::const_iterator end  = mSceneNodes.end();

		while( itor != end )
		{
			(*itor)->translate( Vector3::UNIT_X * timeSinceLast * fMovSpeed, Node::TS_LOCAL );
			++itor;
		}
	}

	void defragmentBatches()
	{
		//Defragment batches is used after many InstancedEntities were removed (and you won't
		//be requesting more). However, then the optimize cull option is on, it can cause
		//quite a perf. boost on large batches (i.e. VTF) even if not a single instance was ever removed.
		if( mCurrentManager )
			mCurrentManager->defragmentBatches( mDefragmentOptimumCull->isChecked() );
	}

	void setupGUI()
	{
		mTechniqueMenu = mTrayMgr->createLongSelectMenu(
			TL_TOPLEFT, "TechniqueSelectMenu", "Technique", 300, 200, 5);
		for( int i=0; i<NUM_TECHNIQUES; ++i )
			mTechniqueMenu->addItem( c_instancingTechniques[i] );

		//Check box to move the units
		mMoveInstances = mTrayMgr->createCheckBox(TL_TOPRIGHT, "MoveInstances", "Move Instances", 175);
		mMoveInstances->setChecked(false);

		//Check box to animate the units
		mAnimateInstances = mTrayMgr->createCheckBox(TL_TOPRIGHT, "AnimateInstances",
														"Animate Instances", 175);
		mAnimateInstances->setChecked(false);

		//Controls to control batch defragmentation on the fly
		mDefragmentBatches =  mTrayMgr->createButton(TL_TOPRIGHT, "DefragmentBatches",
															"Defragment Batches", 175);
		mDefragmentOptimumCull = mTrayMgr->createCheckBox(TL_TOPRIGHT, "DefragmentOptimumCull",
															"Optimum Cull", 175);
		mDefragmentOptimumCull->setChecked(true);

		mTrayMgr->showCursor();
	}

	void itemSelected(SelectMenu* menu)
	{
		if (menu == mTechniqueMenu)
		{
			clearScene();
			switchInstancingTechnique();
		}
		/*else if (menu == mLightingMenu) handleShadowTypeChanged();
		else if (menu == mProjectionMenu) handleProjectionChanged();
		else if (menu == mMaterialMenu) handleMaterialChanged();*/
	}

	void buttonHit( Button* button )
	{
		if( button == mDefragmentBatches ) defragmentBatches();
	}

	//You can also use a union type to switch between Entity and InstancedEntity almost flawlessly:
	/*
	union FusionEntity
	{
		Entity			entity
		InstancedEntity	instancedEntity;
	};
	*/
	const int NUM_INST_ROW;
	const int NUM_INST_COLUMN;
	int								mInstancingTechnique;
	int								mCurrentMesh;
	std::vector<MovableObject*>		mEntities;
	std::vector<SceneNode*>			mSceneNodes;
	std::vector<AnimationState*>	mAnimations;
	InstanceManager					*mInstanceManagers[NUM_TECHNIQUES-1];
	InstanceManager					*mCurrentManager;

	SelectMenu						*mTechniqueMenu;
	CheckBox						*mMoveInstances;
	CheckBox						*mAnimateInstances;
	Button							*mDefragmentBatches;
	CheckBox						*mDefragmentOptimumCull;
};

#endif