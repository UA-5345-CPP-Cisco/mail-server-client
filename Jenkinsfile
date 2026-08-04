pipeline {
    agent any

    options {
        skipDefaultCheckout(true)
        disableConcurrentBuilds()
        timestamps()
        timeout(time: 45, unit: 'MINUTES')
    }

    /*
     * Check GitHub approximately every five minutes.
     *
     * "H" distributes polling times across Jenkins jobs,
     * avoiding all jobs polling at exactly the same second.
     */
    triggers {
        pollSCM('H/5 * * * *')
    }

    environment {
        /*
         * Jenkins Secret Text credential containing the
         * private IP address or DNS name of the Docker machine.
         */
        DEPLOY_HOST = '10.26.0.53'

        DEPLOY_DIR = '/opt/mail-server-client'
        SSH_PORT   = '22'
    }

    stages {
        stage('Checkout main') {
            steps {
                deleteDir()

                /*
                 * Checks out the exact repository and revision
                 * associated with this Pipeline job.
                 */
                checkout scm

                sh '''#!/usr/bin/env bash
                    set -Eeuo pipefail

                    echo "Checked out commit:"
                    git log -1 --oneline

                    echo "Current branch:"
                    git branch --show-current || true
                '''
            }
        }

        stage('Validate repository') {
            steps {
                sh '''#!/usr/bin/env bash
                    set -Eeuo pipefail

                    test -f docker-compose.yml
                    test -f apps/mail-server/Dockerfile
                    test -f apps/smtp-server/Dockerfile

                    echo "Required deployment files are present."
                '''
            }
        }

        stage('Deploy to Docker machine') {
            steps {
                withCredentials([
                    sshUserPrivateKey(
                        credentialsId: 'docker-machine-ssh',
                        keyFileVariable: 'SSH_KEY',
                        usernameVariable: 'SSH_USER'
                    )
                ]) {
                    sh '''#!/usr/bin/env bash
                        set -Eeuo pipefail

                        TARGET="${SSH_USER}@${DEPLOY_HOST}"

                        SSH_OPTIONS=(
                            -i "$SSH_KEY"
                            -p "$SSH_PORT"
                            -o BatchMode=yes
                            -o StrictHostKeyChecking=yes
                            -o ConnectTimeout=15
                        )

                        echo "Testing connection to ${DEPLOY_HOST}..."

                        ssh "${SSH_OPTIONS[@]}" "$TARGET" \
                            'echo "SSH connection successful."'

                        echo "Preparing deployment directory..."

                        ssh "${SSH_OPTIONS[@]}" "$TARGET" \
                            "mkdir -p '${DEPLOY_DIR}/data'"

                        /*
                         * Configure rsync to use the Jenkins SSH credential.
                         */
                        printf -v RSYNC_RSH \
                            'ssh -i %q -p %q -o BatchMode=yes -o StrictHostKeyChecking=yes -o ConnectTimeout=15' \
                            "$SSH_KEY" "$SSH_PORT"

                        export RSYNC_RSH

                        echo "Copying repository files..."

                        /*
                         * Remove files on the Docker machine when they were
                         * removed from Git, but preserve persistent data.
                         */
                        rsync \
                            --archive \
                            --compress \
                            --delete \
                            --exclude='.git/' \
                            --exclude='data/' \
                            ./ "${TARGET}:${DEPLOY_DIR}/"

                        /*
                         * Copy initial data files only when an equivalent
                         * file does not already exist remotely.
                         */
                        if [[ -d data ]]; then
                            rsync \
                                --archive \
                                --compress \
                                --ignore-existing \
                                data/ "${TARGET}:${DEPLOY_DIR}/data/"
                        fi

                        echo "Starting Docker Compose deployment..."

                        ssh "${SSH_OPTIONS[@]}" "$TARGET" \
                            "DEPLOY_DIR='${DEPLOY_DIR}' bash -s" <<'REMOTE_SCRIPT'
                        set -Eeuo pipefail

                        cd "$DEPLOY_DIR"

                        echo "Validating Compose configuration..."
                        docker compose config --quiet

                        echo "Building and starting services..."
                        docker compose up \
                            --detach \
                            --build \
                            --remove-orphans

                        echo "Service status:"
                        docker compose ps
REMOTE_SCRIPT
                    '''
                }
            }
        }
    }

    post {
        success {
            echo 'Deployment completed successfully.'
        }

        failure {
            echo 'Deployment failed. Review the Jenkins console output.'
        }
    }
}