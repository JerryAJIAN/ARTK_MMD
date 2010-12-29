//*******************
// ARToolKit���b�p�[
//*******************

#include	<math.h>
#include	"ARTK.h"
#include	<string.h>
#include	"../ErrorMes.h"


cARTK		g_clARTK;

//================
// �R���X�g���N�^
//================
cARTK::cARTK( void ) : m_pARTImage( NULL ), m_pArglSettings( NULL ), m_bMarkerFound( false )
{
}

//==============
// �f�X�g���N�^
//==============
cARTK::~cARTK( void )
{
	cleanup();
}

//========
// ������
//========
bool cARTK::initialize( void )
{
	const char	*szCameraParamFName = "../Data/camera_para.dat";
	const char	*szPattFName  = "../Data/patt.00";
#ifdef _WIN32
	char		*szVConfFName = "../Data/WDM_camera_flipV.xml";
#else
	char		*szVConfFName = "";
#endif

	ARParam			sCamParamTemp;
	int				iCamImgSizeX,
					iCamImgSizeY;

	// �J�����f�o�C�X�̃I�[�v��
	if( arVideoOpen( szVConfFName ) < 0 )
	{
		ErrorMessage( "Unable to open connection to camera.\n" );
		return false;
	}

	// �J�����摜�̃T�C�Y���擾
	if( arVideoInqSize( &iCamImgSizeX, &iCamImgSizeY ) < 0 ) return false;

	// �J�����p�����[�^�t�@�C���̓Ǎ���
	if( arParamLoad( szCameraParamFName, 1, &sCamParamTemp ) < 0 )
	{
		ErrorMessage( "Error loading parameter file for camera.\n" );
		return false;
	}

	// �J�����p�����[�^�̃T�C�Y������ύX
	arParamChangeSize( &sCamParamTemp, iCamImgSizeX, iCamImgSizeY, &m_sCameraParam );

	// ���C�u�������̃J�����p�����[�^�̏�����
	arInitCparam( &m_sCameraParam );

	// �J�����摜�̃L���v�`�����J�n
	if( arVideoCapStart() != 0 )
	{
		ErrorMessage( "Unable to begin camera data capture.\n" );
		return false;
	}

	// �}�[�J�[�p�^�[���t�@�C���̓Ǎ���
    if( (m_iPattID = arLoadPatt( szPattFName )) < 0 )
    {
		ErrorMessage( "Pattern file load error !!\n" );
		return false;
    }

	m_uiARTImageSize = sizeof(ARUint8) * iCamImgSizeX * iCamImgSizeY * 3;
	m_pARTImage = (ARUint8 *)malloc( m_uiARTImageSize );
	memset( m_pARTImage, 0, m_uiARTImageSize );

	m_dViewScaleFactor = 0.16;
	m_dPattWidth = 40.0;
	m_bFirstTime = true;

	return true;
}

//==============
// argl�̏�����
//==============
bool cARTK::initArgl( void )
{
	// argl��OpenGL�̌��݂̃R���e�L�X�g�p�ɃZ�b�g�A�b�v
	if( (m_pArglSettings = arglSetupForCurrentContext()) == NULL )
	{
		ErrorMessage( "arglSetupForCurrentContext() returned error.\n" );
		return false;
	}

	double	c = cos( 3.1515926 * 0.5 );
	double	s = sin( 3.1515926 * 0.5 );
	m_dmatRotX[ 0] = 1.0; m_dmatRotX[ 1] = 0.0; m_dmatRotX[ 2] = 0.0; m_dmatRotX[ 3] = 0.0;
	m_dmatRotX[ 4] = 0.0; m_dmatRotX[ 5] =   c; m_dmatRotX[ 6] =   s; m_dmatRotX[ 7] = 0.0;
	m_dmatRotX[ 8] = 0.0; m_dmatRotX[ 9] =  -s; m_dmatRotX[10] =   c; m_dmatRotX[11] = 0.0;
	m_dmatRotX[12] = 0.0; m_dmatRotX[13] = 0.0; m_dmatRotX[14] = 0.0; m_dmatRotX[15] = 1.0;

	MatrixRotationX( m_matRotX, -3.1415926f * 0.5f );

	return true;
}

//==================
// �}�[�J�[���o����
//==================
bool cARTK::update( void )
{
	ARUint8			*pImage;
	ARMarkerInfo	*pMarkerInfo;
    int             iNumDetectedMarker;

	// �J�����摜�̎擾
	if( (pImage = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return false;
    }
	memcpy( m_pARTImage, pImage, m_uiARTImageSize );

	m_bMarkerFound = false;

	// �J�����摜����}�[�J�[�����o
	if( arDetectMarker( m_pARTImage, 130, &pMarkerInfo, &iNumDetectedMarker ) < 0 )
	{
		exit( -1 );
	}

	// ���o���ꂽ�}�[�J�[���̒������ԐM�����̍������̂�T��
	int		k = -1;
	for( int j = 0 ; j < iNumDetectedMarker ; j++ )
	{
		if( pMarkerInfo[j].id == m_iPattID )
		{
			if( k == -1 || pMarkerInfo[j].cf > pMarkerInfo[k].cf )	k = j;
		}
	}

	if( k != -1 )
	{
		// �J�����̃g�����X�t�H�[���s����擾
		if( m_bFirstTime )
			arGetTransMat( &(pMarkerInfo[k]), m_dPattCenter, m_dPattWidth, m_dPattTransMat );
		else
			arGetTransMatCont( &(pMarkerInfo[k]), m_dPattTransMat, m_dPattCenter, m_dPattWidth, m_dPattTransMat );

		m_bFirstTime = false;

		m_bMarkerFound = true;
	}

	// ���̃J�����摜�̃L���v�`�����J�n
	arVideoCapNext();

	return true;
}

//==============
// �`�掞�̏���
//==============
void cARTK::display( void )
{
	// �J�����摜��`��
	arglDispImage( m_pARTImage, &m_sCameraParam, 1.0, m_pArglSettings );	// zoom = 1.0.
}

//====================
// �J�����̈ʒu�̎擾
//====================
void cARTK::getCameraPos( Vector3 *pvec3CamPos )
{
	double		dInvPattTransMat[3][4];

	arUtilMatInv( m_dPattTransMat, dInvPattTransMat );

	pvec3CamPos->x = (float)dInvPattTransMat[0][3];
	pvec3CamPos->y = (float)dInvPattTransMat[1][3];
	pvec3CamPos->z = (float)dInvPattTransMat[2][3];
	Vector3Transform( pvec3CamPos, pvec3CamPos, m_matRotX );

	pvec3CamPos->x *= (float)m_dViewScaleFactor;
	pvec3CamPos->y *= (float)m_dViewScaleFactor;
	pvec3CamPos->z *= (float)m_dViewScaleFactor;
}

//============================
// �v���W�F�N�V�����s��̎擾
//============================
void cARTK::getProjectionMat( double dMat[16] )
{
								//        near  far
	arglCameraFrustumRH( &m_sCameraParam, 0.1, 400.0, dMat );
}

//========================
// ���f���r���[�s��̎擾
//========================
void cARTK::getModelViewMat( double dMat[16] )
{
	double	dmatTemp[16];

	arglCameraViewRH( m_dPattTransMat, dmatTemp, m_dViewScaleFactor );

	for( int i = 0 ; i < 4 ; i++ )
	{
		int		idx = i << 2;
		dMat[idx + 0] =	m_dmatRotX[idx] * dmatTemp[0] + m_dmatRotX[idx + 1] * dmatTemp[4] + m_dmatRotX[idx + 2] * dmatTemp[ 8] + m_dmatRotX[idx + 3] * dmatTemp[12];
		dMat[idx + 1] =	m_dmatRotX[idx] * dmatTemp[1] + m_dmatRotX[idx + 1] * dmatTemp[5] + m_dmatRotX[idx + 2] * dmatTemp[ 9] + m_dmatRotX[idx + 3] * dmatTemp[13];
		dMat[idx + 2] =	m_dmatRotX[idx] * dmatTemp[2] + m_dmatRotX[idx + 1] * dmatTemp[6] + m_dmatRotX[idx + 2] * dmatTemp[10] + m_dmatRotX[idx + 3] * dmatTemp[14];
		dMat[idx + 3] =	m_dmatRotX[idx] * dmatTemp[3] + m_dmatRotX[idx + 1] * dmatTemp[7] + m_dmatRotX[idx + 2] * dmatTemp[11] + m_dmatRotX[idx + 3] * dmatTemp[15];
	}
}

//==============
// �g�嗦�̑���
//==============
void cARTK::addViewScaleFactor( double dAdd )
{
	m_dViewScaleFactor += dAdd;

	// �������������قǃ��f���͑傫���\�������
	if( m_dViewScaleFactor < 0.01 )	m_dViewScaleFactor = 0.01;
	if( 0.80 < m_dViewScaleFactor )	m_dViewScaleFactor = 0.80;
}

//======================
// ���C�u�����̏I������
//======================
void cARTK::cleanup( void )
{
	if( m_pARTImage )
	{
		free( m_pARTImage );
		m_pARTImage = NULL;
	}

	if( m_pArglSettings )
	{
		arglCleanup( m_pArglSettings );
		m_pArglSettings = NULL;
	}
	arVideoCapStop();
	arVideoClose();
}
